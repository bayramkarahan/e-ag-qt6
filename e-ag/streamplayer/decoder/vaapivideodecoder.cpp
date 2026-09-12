#include "vaapivideodecoder.h"

#include <QDebug>
#include <QFile>

extern "C"
{
#include <libavutil/hwcontext.h>
}

VaapiVideoDecoder::VaapiVideoDecoder(QObject *parent)
    : AbstractVideoDecoder(parent)
{
}

VaapiVideoDecoder::~VaapiVideoDecoder()
{
    stop();
}
void VaapiVideoDecoder::setSettings(const StreamSettings &settings)
{
    m_settings=settings;
}
bool VaapiVideoDecoder::isSupported()
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

enum AVPixelFormat VaapiVideoDecoder::getHwFormat(
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


void VaapiVideoDecoder::stop()
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
bool VaapiVideoDecoder::start()
{

    int ret = av_hwdevice_ctx_create(
                &m_hwDevice,
                AV_HWDEVICE_TYPE_VAAPI,
                "/dev/dri/renderD128",
                nullptr,
                0);

    if (ret < 0)
    {
        qDebug() << "Cannot create VAAPI device.";
        return false;
    }

    qDebug() << "VAAPI device created.";

    //-----------------------------------------
    // H264 Decoder
    //-----------------------------------------

    const AVCodec *codec =
            avcodec_find_decoder(AV_CODEC_ID_H264);

    if (!codec)
    {
        qDebug() << "H264 decoder not found.";
        return false;
    }

    m_codecContext = avcodec_alloc_context3(codec);

    if (!m_codecContext)
    {
        qDebug() << "Cannot allocate codec context.";
        return false;
    }

    m_codecContext->flags  |= AV_CODEC_FLAG_LOW_DELAY;
    //m_codecContext->skip_loop_filter = AVDISCARD_ALL;
    //-----------------------------------------
    // VAAPI
    //-----------------------------------------

    m_codecContext->hw_device_ctx =
            av_buffer_ref(m_hwDevice);

    m_codecContext->get_format =
            getHwFormat;

    //-----------------------------------------
    // Open decoder
    //-----------------------------------------
     ret = avcodec_open2(
                m_codecContext,
                codec,
                nullptr);

    if (ret < 0)
    {
        char err[256];

        av_strerror(ret, err, sizeof(err));

        qDebug() << "Cannot open decoder:" << err;

        return false;
    }

    //-----------------------------------------

    m_packet = av_packet_alloc();
    m_frame = av_frame_alloc();
    m_swFrame = av_frame_alloc();

    qDebug() << "VAAPI decoder opened.";


qDebug() << "Decoder ready";
    return true;
}


void VaapiVideoDecoder::decode(const QByteArray &packet,
                               qint64 pts,
                               bool marker)
{
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

void VaapiVideoDecoder::decodePacket(const QByteArray &nal,
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
        char err[256];
        av_strerror(ret, err, sizeof(err));
        qDebug() << "send_packet:" << err;
        return;
    }

    while (true)
    {
        ret = avcodec_receive_frame(m_codecContext, m_frame);

        if (ret == AVERROR(EAGAIN) ||
            ret == AVERROR_EOF)
            break;

        if (ret < 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));
            qDebug() << "receive_frame:" << err;
            break;
        }

        av_frame_unref(m_swFrame);

        ret = av_hwframe_transfer_data(m_swFrame,
                                       m_frame,
                                       0);

        if (ret < 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));
            qDebug() << "transfer:" << err;
            av_frame_unref(m_frame);
            continue;
        }

        MediaFrame frame;

        frame.width  = m_swFrame->width;
        frame.height = m_swFrame->height;
        frame.pts    = pts;

        frame.buffer.reset(new MediaBuffer);

        MediaBuffer *buffer = frame.buffer.data();

        //------------------------------------------------------------------
        // NV12
        //------------------------------------------------------------------
        if (m_swFrame->format == AV_PIX_FMT_NV12)
        {
            frame.format = PixelFormat::NV12;

            int ySize  = m_swFrame->linesize[0] * frame.height;
            int uvSize = m_swFrame->linesize[1] * (frame.height / 2);

            buffer->storage.resize(ySize + uvSize);

            buffer->planeCount = 2;

            buffer->plane[0] =
                    reinterpret_cast<uint8_t*>(buffer->storage.data());

            memcpy(buffer->plane[0],
                   m_swFrame->data[0],
                   ySize);

            buffer->stride[0]    = m_swFrame->linesize[0];
            buffer->planeSize[0] = ySize;

            buffer->plane[1] =
                    buffer->plane[0] + ySize;

            memcpy(buffer->plane[1],
                   m_swFrame->data[1],
                   uvSize);

            buffer->stride[1]    = m_swFrame->linesize[1];
            buffer->planeSize[1] = uvSize;
        }

        //------------------------------------------------------------------
        // YUV420P
        //------------------------------------------------------------------
        else if (m_swFrame->format == AV_PIX_FMT_YUV420P)
        {
            frame.format = PixelFormat::YUV420P;

            int size =
                av_image_get_buffer_size(
                    AV_PIX_FMT_YUV420P,
                    frame.width,
                    frame.height,
                    1);

            buffer->storage.resize(size);

            av_image_fill_arrays(
                        buffer->plane,
                        buffer->stride,
                        reinterpret_cast<uint8_t*>(buffer->storage.data()),
                        AV_PIX_FMT_YUV420P,
                        frame.width,
                        frame.height,
                        1);

            av_image_copy(
                        buffer->plane,
                        buffer->stride,
                        (const uint8_t**)m_swFrame->data,
                        m_swFrame->linesize,
                        AV_PIX_FMT_YUV420P,
                        frame.width,
                        frame.height);

            buffer->planeCount = 3;

            buffer->planeSize[0] =
                    buffer->stride[0] * frame.height;

            buffer->planeSize[1] =
                    buffer->stride[1] * (frame.height / 2);

            buffer->planeSize[2] =
                    buffer->stride[2] * (frame.height / 2);
        }

        //------------------------------------------------------------------
        // Diğer formatlar
        //------------------------------------------------------------------
        else
        {
            frame.format = PixelFormat::NV12;

            int ySize = frame.width * frame.height;
            int uvSize = ySize / 2;

            buffer->storage.resize(ySize + uvSize);

            buffer->planeCount = 2;

            buffer->plane[0] =
                    reinterpret_cast<uint8_t*>(buffer->storage.data());

            buffer->plane[1] =
                    buffer->plane[0] + ySize;

            buffer->stride[0] = frame.width;
            buffer->stride[1] = frame.width;

            buffer->planeSize[0] = ySize;
            buffer->planeSize[1] = uvSize;

            m_sws = sws_getCachedContext(
                        m_sws,
                        frame.width,
                        frame.height,
                        (AVPixelFormat)m_swFrame->format,
                        frame.width,
                        frame.height,
                        AV_PIX_FMT_NV12,
                        SWS_FAST_BILINEAR,
                        nullptr,
                        nullptr,
                        nullptr);

            sws_scale(
                        m_sws,
                        m_swFrame->data,
                        m_swFrame->linesize,
                        0,
                        frame.height,
                        buffer->plane,
                        buffer->stride);
        }

        emit frameReady(frame);

        av_frame_unref(m_frame);
    }
}

/*
void VaapiVideoDecoder::decodePacket(const QByteArray &nal,
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
        char err[256];
        av_strerror(ret, err, sizeof(err));

        qDebug() << "send_packet:" << err;
        return;
    }

    while (true)
    {
        ret = avcodec_receive_frame(m_codecContext, m_frame);

        if (ret == AVERROR(EAGAIN) ||
            ret == AVERROR_EOF)
        {
            break;
        }

        if (ret < 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));

            qDebug() << "receive_frame:" << err;
            break;
        }

        av_frame_unref(m_swFrame);

        ret = av_hwframe_transfer_data(
                    m_swFrame,
                    m_frame,
                    0);

        if (ret < 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));

            qDebug() << "transfer:" << err;
            continue;
        }

        MediaFrame frame;

        frame.width  = m_swFrame->width;
        frame.height = m_swFrame->height;
        frame.format = PixelFormat::YUV420P;
        frame.pts    = pts;

        frame.buffer.reset(new MediaBuffer);

        MediaBuffer *buffer = frame.buffer.data();

        int size = av_image_get_buffer_size(
                    AV_PIX_FMT_YUV420P,
                    frame.width,
                    frame.height,
                    1);

        buffer->storage.resize(size);

        av_image_fill_arrays(
                    buffer->plane,
                    buffer->stride,
                    reinterpret_cast<uint8_t*>(buffer->storage.data()),
                    AV_PIX_FMT_YUV420P,
                    frame.width,
                    frame.height,
                    1);

        // NV12 -> YUV420P converter
        m_sws = sws_getCachedContext(
                    m_sws,
                    frame.width,
                    frame.height,
                    (AVPixelFormat)m_swFrame->format,
                    frame.width,
                    frame.height,
                    AV_PIX_FMT_YUV420P,
                    SWS_FAST_BILINEAR,
                    nullptr,
                    nullptr,
                    nullptr);

        if (!m_sws)
        {
            qDebug() << "Cannot create sws context";
            av_frame_unref(m_frame);
            continue;
        }

        sws_scale(
                    m_sws,
                    m_swFrame->data,
                    m_swFrame->linesize,
                    0,
                    frame.height,
                    buffer->plane,
                    buffer->stride);

        buffer->planeCount = 3;

        buffer->planeSize[0] =
                buffer->stride[0] * frame.height;

        buffer->planeSize[1] =
                buffer->stride[1] * (frame.height / 2);

        buffer->planeSize[2] =
                buffer->stride[2] * (frame.height / 2);

        emit frameReady(frame);

        av_frame_unref(m_frame);
    }
}
*/

/*void VaapiVideoDecoder::decodePacket(const QByteArray &nal,
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
        char err[256];
        av_strerror(ret, err, sizeof(err));

        qDebug() << "send_packet:" << err;
        return;
    }

    while (true)
    {
        ret = avcodec_receive_frame(m_codecContext, m_frame);

        if (ret == AVERROR(EAGAIN) ||
            ret == AVERROR_EOF)
        {
            break;
        }

        if (ret < 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));

            qDebug() << "receive_frame:" << err;
            break;
        }

        av_frame_unref(m_swFrame);

        ret = av_hwframe_transfer_data(
                    m_swFrame,
                    m_frame,
                    0);

        if (ret < 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));

            qDebug() << "transfer:" << err;
            continue;
        }

        MediaFrame frame;

        //-----------------------------------------------------
        // Hedef çözünürlük
        //-----------------------------------------------------

        int dstWidth  = m_swFrame->width;
        int dstHeight = m_swFrame->height;

        if (m_settings.video.width > 0 &&
            m_settings.video.height > 0)
        {
            dstWidth  = m_settings.video.width;
            dstHeight = m_settings.video.height;
        }

        //-----------------------------------------------------
        // swscale oluştur
        //-----------------------------------------------------

        m_sws = sws_getCachedContext(
                    m_sws,
                    m_swFrame->width,
                    m_swFrame->height,
                    (AVPixelFormat)m_swFrame->format,
                    dstWidth,
                    dstHeight,
                    AV_PIX_FMT_YUV420P,
                    SWS_BILINEAR,
                    nullptr,
                    nullptr,
                    nullptr);

        if (!m_sws)
        {
            qDebug() << "Cannot create sws context";
            continue;
        }

        //-----------------------------------------------------

        frame.width  = dstWidth;
        frame.height = dstHeight;
        frame.format = PixelFormat::YUV420P;
        frame.pts    = pts;

        frame.buffer.reset(new MediaBuffer);

        MediaBuffer *buffer = frame.buffer.data();

        int size = av_image_get_buffer_size(
                        AV_PIX_FMT_YUV420P,
                        dstWidth,
                        dstHeight,
                        1);

        buffer->storage.resize(size);

        av_image_fill_arrays(
                    buffer->plane,
                    buffer->stride,
                    reinterpret_cast<uint8_t*>(buffer->storage.data()),
                    AV_PIX_FMT_YUV420P,
                    dstWidth,
                    dstHeight,
                    1);

        /*qDebug()
                << "HW"
                << av_get_pix_fmt_name((AVPixelFormat)m_frame->format)
                << "SW"
                << av_get_pix_fmt_name((AVPixelFormat)m_swFrame->format)
                << "OUT"
                << dstWidth
                << "x"
                << dstHeight;*/

        //-----------------------------------------------------
        // NV12 -> YUV420P + Resize
        //-----------------------------------------------------

   /*     sws_scale(
                m_sws,
                m_swFrame->data,
                m_swFrame->linesize,
                0,
                m_swFrame->height,
                buffer->plane,
                buffer->stride);

        //-----------------------------------------------------

        buffer->planeCount = 3;

        buffer->planeSize[0] =
                buffer->stride[0] * dstHeight;

        buffer->planeSize[1] =
                buffer->stride[1] * (dstHeight / 2);

        buffer->planeSize[2] =
                buffer->stride[2] * (dstHeight / 2);

        emit frameReady(frame);

        av_frame_unref(m_frame);
    }
}
*/
/*void VaapiVideoDecoder::decodePacket(const QByteArray &nal,
                                    qint64 pts)
{
    av_packet_unref(m_packet);

    if (!m_codecContext)
        return;
/*
    const uint8_t *p =
        reinterpret_cast<const uint8_t*>(nal.constData());

    int offset = (p[2] == 1) ? 3 : 4;

    qDebug()
        << "DECODE NAL"
        << (p[offset] & 0x1f)
        << nal.size();

    qDebug().noquote()
        << nal.left(16).toHex(' ');
*/
/*
    if (av_new_packet(m_packet, nal.size()) < 0)
        return;

    memcpy(m_packet->data,
           nal.constData(),
           nal.size());
    m_packet->pts = pts;
    m_packet->dts = pts;
    ///m_packet->pts = AV_NOPTS_VALUE;
    ///m_packet->dts = AV_NOPTS_VALUE;
    int ret = avcodec_send_packet(
                m_codecContext,
                m_packet);

    if (ret < 0)
    {
        char err[256];
        av_strerror(ret, err, sizeof(err));

        qDebug()
                << "send_packet:"
                << err;

        return;
    }

    while (true)
    {
        ret = avcodec_receive_frame(
                    m_codecContext,
                    m_frame);

        if (ret == AVERROR(EAGAIN) ||
            ret == AVERROR_EOF)
        {
            break;
        }

        if (ret < 0)
        {
            char err[256];

            av_strerror(ret, err, sizeof(err));

            qDebug()
                    << "receive_frame:"
                    << err;

            break;
        }

        av_frame_unref(m_swFrame);

        ret = av_hwframe_transfer_data(
                    m_swFrame,
                    m_frame,
                    0);

        if (ret < 0)
        {
            char err[256];

            av_strerror(ret, err, sizeof(err));

            qDebug()
                    << "transfer:"
                    << err;

            continue;
        }

       /*qDebug()
                << "VAAPI frame"
                << m_swFrame->width
                << "x"
                << m_swFrame->height;
*/
  /*      MediaFrame frame;

        frame.width  = m_swFrame->width;
        frame.height = m_swFrame->height;
        frame.format = PixelFormat::YUV420P;
       //  frame.format = PixelFormat::NV12
        frame.pts    = pts;
        ///frame.pts = m_swFrame->best_effort_timestamp;
        frame.buffer.reset(new MediaBuffer);

        MediaBuffer *buffer = frame.buffer.data();

        int size =
                av_image_get_buffer_size(
                    AV_PIX_FMT_YUV420P,
                    frame.width,
                    frame.height,
                    1);

        buffer->storage.resize(size);

        av_image_fill_arrays(
                    buffer->plane,
                    buffer->stride,
                    reinterpret_cast<uint8_t*>(
                        buffer->storage.data()),
                    AV_PIX_FMT_YUV420P,
                    frame.width,
                    frame.height,
                    1);
        qDebug()
                << "HW"
                << av_get_pix_fmt_name((AVPixelFormat)m_frame->format)
                << "SW"
                << av_get_pix_fmt_name((AVPixelFormat)m_swFrame->format);

        av_image_copy(
                    buffer->plane,
                    buffer->stride,
                    (const uint8_t**)m_swFrame->data,
                    m_swFrame->linesize,
                    AV_PIX_FMT_YUV420P,
                    frame.width,
                    frame.height);

        buffer->planeCount = 3;

        buffer->planeSize[0] =
                buffer->stride[0] * frame.height;

        buffer->planeSize[1] =
                buffer->stride[1] * (frame.height / 2);

        buffer->planeSize[2] =
                buffer->stride[2] * (frame.height / 2);

        emit frameReady(frame);
        av_frame_unref(m_frame);
    }

}*/
