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

/*
void VaapiZeroCopyDecoder::decode(const QByteArray &packet,
                               qint64 pts,
                               bool marker)
{
   /* qDebug()
        << "VaapiZeroCopyDecoder VIDEO INPUT"
        << "size =" << packet.size()
        << "pts =" << pts
        << "marker =" << marker;*/
/*
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

*/


void VaapiZeroCopyDecoder::decode(const QByteArray &packet,
                                  qint64 pts,
                                  bool marker)
{
    Q_UNUSED(marker);

    if (packet.size() < 5)
        return;

    const uint8_t *p =
        reinterpret_cast<const uint8_t *>(packet.constData());

    // Annex-B başlangıç kodunu belirle: 00 00 01 veya 00 00 00 01
    int offset = -1;

    if (p[0] == 0 && p[1] == 0 && p[2] == 1)
    {
        offset = 3;
    }
    else if (packet.size() >= 5 &&
             p[0] == 0 && p[1] == 0 &&
             p[2] == 0 && p[3] == 1)
    {
        offset = 4;
    }

    if (offset < 0 || offset >= packet.size())
        return;

    const int nalType = p[offset] & 0x1f;

    switch (nalType)
    {
    case 7: // SPS
        m_sps = packet;
        return;

    case 8: // PPS
        m_pps = packet;
        return;

    case 5: // IDR
    {
        if (m_sps.isEmpty() || m_pps.isEmpty())
        {
            qWarning() << "IDR received without SPS/PPS";
            return;
        }

        if (!m_decoderReady)
        {
            if (!start())
                return;

            m_decoderReady = true;
        }

        // SPS + PPS + IDR tek Annex-B paketi olarak gönderilir.
        QByteArray accessUnit;
        accessUnit.reserve(
            m_sps.size() + m_pps.size() + packet.size());

        accessUnit.append(m_sps);
        accessUnit.append(m_pps);
        accessUnit.append(packet);

        decodePacket(accessUnit, pts);

        m_started = true;
        return;
    }

    case 1: // P frame
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
    if (!m_codecContext || !m_packet || !m_frame)
        return;

    if (nal.isEmpty())
        return;

    av_packet_unref(m_packet);

    int ret = av_new_packet(m_packet, nal.size());
    if (ret < 0)
    {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
        av_strerror(ret, errbuf, sizeof(errbuf));

        qWarning() << "av_new_packet failed:"
                   << errbuf
                   << "size:" << nal.size();
        return;
    }

    memcpy(m_packet->data, nal.constData(), nal.size());

    m_packet->pts = pts;
    m_packet->dts = pts;

    // Gelen paketin NAL türünü tanıla.
    int offset = -1;

    const auto *p =
        reinterpret_cast<const uint8_t *>(nal.constData());

    if (nal.size() >= 4 &&
        p[0] == 0 && p[1] == 0 && p[2] == 1)
    {
        offset = 3;
    }
    else if (nal.size() >= 5 &&
             p[0] == 0 && p[1] == 0 &&
             p[2] == 0 && p[3] == 1)
    {
        offset = 4;
    }

    const int nalType =
        (offset >= 0 && offset < nal.size())
            ? (p[offset] & 0x1f)
            : -1;

    ret = avcodec_send_packet(m_codecContext, m_packet);

    if (ret < 0)
    {
        char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
        av_strerror(ret, errbuf, sizeof(errbuf));

        qWarning() << "VAAPI SEND ERROR:"
                   << errbuf
                   << "size:" << m_packet->size
                   << "NAL type:" << nalType
                   << "first bytes:"
                   << nal.left(12).toHex()
                   << "pts:" << pts
                   << "ret:" << ret;

        return;
    }

    while (true)
    {
        ret = avcodec_receive_frame(m_codecContext, m_frame);

        if (ret == AVERROR(EAGAIN))
            break;

        if (ret == AVERROR_EOF)
        {
            qWarning() << "VAAPI decoder reached EOF";
            break;
        }

        if (ret < 0)
        {
            char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
            av_strerror(ret, errbuf, sizeof(errbuf));

            qWarning() << "VAAPI RECEIVE ERROR:"
                       << errbuf
                       << "ret:" << ret;
            break;
        }

        const bool validFormat =
            m_frame->format == AV_PIX_FMT_VAAPI;

        const bool validFramesContext =
            m_frame->hw_frames_ctx != nullptr;

        const bool validSurface =
            m_frame->data[3] != nullptr;

        if (!validFormat ||
            !validFramesContext ||
            !validSurface)
        {
            qWarning() << "Invalid VAAPI hardware frame:"
                       << "format:" << m_frame->format
                       << "expected:" << AV_PIX_FMT_VAAPI
                       << "hw_frames_ctx:"
                       << m_frame->hw_frames_ctx
                       << "data[3]:" << static_cast<const void *>(
                              m_frame->data[3])
                       << "width:" << m_frame->width
                       << "height:" << m_frame->height;

            av_frame_unref(m_frame);
            continue;
        }

        if (!m_hwDevice || !m_hwDevice->data)
        {
            qWarning() << "VAAPI device context is null";
            av_frame_unref(m_frame);
            continue;
        }

        auto *device =
            reinterpret_cast<AVHWDeviceContext *>(
                m_hwDevice->data);

        auto *va =
            reinterpret_cast<AVVAAPIDeviceContext *>(
                device->hwctx);

        if (!va || !va->display)
        {
            qWarning() << "VAAPI display is null";
            av_frame_unref(m_frame);
            continue;
        }

        QSharedPointer<VaapiHardwareFrame> hw(
            new VaapiHardwareFrame);

        hw->frame = av_frame_alloc();

        if (!hw->frame)
        {
            qWarning() << "Cannot allocate hardware frame";
            av_frame_unref(m_frame);
            break;
        }

        ret = av_frame_ref(hw->frame, m_frame);

        if (ret < 0)
        {
            char errbuf[AV_ERROR_MAX_STRING_SIZE] = {};
            av_strerror(ret, errbuf, sizeof(errbuf));

            qWarning() << "av_frame_ref failed:"
                       << errbuf;

            av_frame_free(&hw->frame);
            av_frame_unref(m_frame);
            continue;
        }

        hw->display = va->display;
        hw->surface =
            static_cast<VASurfaceID>(
                reinterpret_cast<uintptr_t>(
                    m_frame->data[3]));

        MediaFrame frame;
        frame.width = m_frame->width;
        frame.height = m_frame->height;
        frame.pts = m_frame->pts;
        frame.hardwareFrame = hw;

        emit frameReady(frame);

        av_frame_unref(m_frame);
    }
}

AbstractVideoDecoder::Type VaapiZeroCopyDecoder::type() const
{
    return Type::VAAPI;
}
