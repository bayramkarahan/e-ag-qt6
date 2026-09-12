#include "vaapizerocopydecoder.h"

#include <QDebug>
#include <QFile>

extern "C"
{
#include <libavutil/hwcontext.h>
}

VaapiZeroCopyDecoder::VaapiZeroCopyDecoder(QObject *parent)
    : AbstractVideoDecoder(parent)
{
}

VaapiZeroCopyDecoder::~VaapiZeroCopyDecoder()
{
    stop();
}
void VaapiZeroCopyDecoder::setSettings(const StreamSettings &settings)
{
    m_settings=settings;
}
bool VaapiZeroCopyDecoder::isSupported()
{
    AVHWDeviceType type =
            av_hwdevice_find_type_by_name("vaapi");

    if (type == AV_HWDEVICE_TYPE_NONE)
    {
        qDebug() << "FFmpeg has no VAAPI support.";
        return false;
    }

    qDebug() << "FFmpeg VAAPI support found.";

    if (!QFile::exists("/dev/dri/renderD128"))
    {
        qDebug() << "/dev/dri/renderD128 not found.";
        return false;
    }

    qDebug() << "VAAPI device exists.";

    return true;
}

enum AVPixelFormat VaapiZeroCopyDecoder::getHwFormat(
        AVCodecContext *,
        const enum AVPixelFormat *pix_fmts)
{
    while (*pix_fmts != AV_PIX_FMT_NONE)
    {
        if (*pix_fmts == AV_PIX_FMT_VAAPI)
            return *pix_fmts;

        ++pix_fmts;
    }

    return AV_PIX_FMT_NONE;
}


void VaapiZeroCopyDecoder::stop()
{
    if (m_packet)
    {
        av_packet_free(&m_packet);
        m_packet = nullptr;
    }

    if (m_frame)
    {
        av_frame_free(&m_frame);
        m_frame = nullptr;
    }

    if (m_swFrame)
    {
        av_frame_free(&m_swFrame);
        m_swFrame = nullptr;
    }

    if (m_codecContext)
    {
        avcodec_free_context(&m_codecContext);
        m_codecContext = nullptr;
    }

    if (m_hwDevice)
    {
        av_buffer_unref(&m_hwDevice);
        m_hwDevice = nullptr;
    }

}
bool VaapiZeroCopyDecoder::start()
{
    int ret;

    //-----------------------------------------
    // Create VAAPI Device
    //-----------------------------------------

    ret = av_hwdevice_ctx_create(
                &m_hwDevice,
                AV_HWDEVICE_TYPE_VAAPI,
                "/dev/dri/renderD128",
                nullptr,
                0);

    if (ret < 0)
    {
        char err[256];
        av_strerror(ret, err, sizeof(err));

        qDebug() << "Cannot create VAAPI device:" << err;
        return false;
    }

    qDebug() << "VAAPI device created.";

    //-----------------------------------------
    // Find decoder
    //-----------------------------------------

    const AVCodec *codec =
            avcodec_find_decoder(AV_CODEC_ID_H264);

    if (!codec)
    {
        qDebug() << "H264 decoder not found.";
        return false;
    }

    qDebug() << "Decoder =" << codec->name;

    //-----------------------------------------
    // Codec Context
    //-----------------------------------------

    m_codecContext =
            avcodec_alloc_context3(codec);

    if (!m_codecContext)
    {
        qDebug() << "Cannot allocate codec context.";
        return false;
    }

    //-----------------------------------------
    // Low latency
    //-----------------------------------------

    m_codecContext->flags |= AV_CODEC_FLAG_LOW_DELAY;

    m_codecContext->thread_count = 1;
    m_codecContext->thread_type = 0;

    //-----------------------------------------
    // VAAPI
    //-----------------------------------------

    m_codecContext->hw_device_ctx =
            av_buffer_ref(m_hwDevice);

    m_codecContext->get_format =
            VaapiZeroCopyDecoder::getHwFormat;

    //-----------------------------------------
    // Open decoder
    //-----------------------------------------

    ret = avcodec_open2(
                m_codecContext,
                codec,
                nullptr);
qDebug() << "hw_frames_ctx =" << m_codecContext->hw_frames_ctx;
    if (ret < 0)
    {
        char err[256];
        av_strerror(ret, err, sizeof(err));

        qDebug() << "Cannot open decoder:" << err;

        return false;
    }

    //-----------------------------------------

    m_packet  = av_packet_alloc();
    m_frame   = av_frame_alloc();
    m_swFrame = av_frame_alloc();

    qDebug() << "VAAPI decoder opened.";

    return true;
}

void VaapiZeroCopyDecoder::decode(const QByteArray &packet,
                               qint64 pts,
                               bool marker)
{
   /* qDebug()
        << "VaapiZeroCopyDecoder VIDEO INPUT"
        << "size =" << packet.size()
        << "pts =" << pts
        << "marker =" << marker;*/

    Q_UNUSED(marker);

    if (packet.size() < 5)
        return;

    const uint8_t *p =
        reinterpret_cast<const uint8_t*>(packet.constData());

    int offset = (p[2] == 1) ? 3 : 4;
    int nalType = p[offset] & 0x1f;

    switch (nalType)
    {
    case 7:     // SPS
        m_sps = packet;
        return;

    case 8:     // PPS
        m_pps = packet;
        return;

    case 5:     // IDR
        if (!m_decoderReady)
        {
            if (!start())
                return;

            m_decoderReady = true;
        }

        if (m_sps.isEmpty() || m_pps.isEmpty())
            return;

        decodePacket(m_sps, pts);
        decodePacket(m_pps, pts);
        decodePacket(packet, pts);

        m_started = true;
        return;

    case 1:     // P Frame
        if (!m_started)
            return;

        decodePacket(packet, pts);
        return;

    default:
        return;
    }
}



void VaapiZeroCopyDecoder::decodePacket(const QByteArray &nal,
                                     qint64 pts)
{

    av_packet_unref(m_packet);

    if (!m_codecContext)
        return;

    if (av_new_packet(m_packet, nal.size()) < 0)
        return;

    memcpy(m_packet->data,
           nal.constData(),
           nal.size());

    m_packet->pts = pts;
    m_packet->dts = pts;

    int ret = avcodec_send_packet(m_codecContext, m_packet);

    if (ret < 0)
    {
        static int errorCount = 0;

        ++errorCount;

        if (errorCount % 100 == 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));

            qDebug() << "send_packet error:"
                     << err
                     << "count:"
                     << errorCount;
        }

        return;
    }


    while (true)
    {
        ret = avcodec_receive_frame(m_codecContext, m_frame);

        if (ret == AVERROR(EAGAIN))
            break;

        if (ret == AVERROR_EOF)
            break;

        if (ret < 0)
        {
            static int errorCount = 0;

            ++errorCount;

            if (errorCount % 100 == 0)
            {
                char err[256];
                av_strerror(ret, err, sizeof(err));

                qDebug() << "receive_frame error:"
                         << err
                         << "count:"
                         << errorCount;
            }

            break;
        }

      /*  static int frameCount = 0;

           if (++frameCount % 100 == 0)
           {
               qDebug() << "DECODED FRAME"
                        << frameCount
                        << "size:"
                        << m_frame->width
                        << "x"
                        << m_frame->height
                        << "format:"
                        << m_frame->format;
           }*/


        av_frame_unref(m_swFrame);

        QSharedPointer<VaapiHardwareFrame> hw(
            new VaapiHardwareFrame);

        hw->frame = av_frame_alloc();

        AVHWDeviceContext *device =
            (AVHWDeviceContext*)m_hwDevice->data;

        AVVAAPIDeviceContext *va =
            (AVVAAPIDeviceContext*)device->hwctx;

        hw->display = va->display;

        av_frame_ref(hw->frame, m_frame);
/***************************/
        AVFrame *avFrame = hw->frame;

        if (!avFrame->hw_frames_ctx)
        {
            qDebug() << "No hw_frames_ctx";
        }
        else
        {
            VASurfaceID surface =
                (VASurfaceID)(uintptr_t)avFrame->data[3];

           /// qDebug() << "Surface =" << surface;
        }
        /*******************************/
        MediaFrame frame;

        frame.width  = m_frame->width;
        frame.height = m_frame->height;
        frame.pts    = pts;

        frame.hardwareFrame = hw;
/**********************************************/
       /// AVFrame *avFrame = hw->frame;

        AVHWFramesContext *frames =
            (AVHWFramesContext*)avFrame->hw_frames_ctx->data;

        AVVAAPIFramesContext *vaFrames =
            (AVVAAPIFramesContext*)frames->hwctx;

        Q_UNUSED(vaFrames);

        VASurfaceID surface =
            (VASurfaceID)(uintptr_t)avFrame->data[3];
        hw->surface = surface;
        ///qDebug() << "VA Surface =" << surface;
        /********************************************/

        emit frameReady(frame);

        av_frame_unref(m_frame);
    }
}

AbstractVideoDecoder::Type VaapiZeroCopyDecoder::type() const
{
    return Type::VAAPI;
}
