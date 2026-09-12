#include "h264decoder.h"

extern "C"
{
#include <libavutil/error.h>
#include <libavutil/imgutils.h>
}
#include <QImage>

extern "C"
{
#include <libswscale/swscale.h>
}

#include <QDebug>
static void saveDecodedFrame(const MediaFrame &frame)
{
    if (!frame.buffer)
        return;

    SwsContext *ctx =
        sws_getContext(
            frame.width,
            frame.height,
            AV_PIX_FMT_YUV420P,
            frame.width,
            frame.height,
            AV_PIX_FMT_RGBA,
            SWS_FAST_BILINEAR,
            nullptr,
            nullptr,
            nullptr);

    if (!ctx)
        return;

    QImage image(
        frame.width,
        frame.height,
        QImage::Format_RGBA8888);

    uint8_t *dstData[4] =
    {
        image.bits(),
        nullptr,
        nullptr,
        nullptr
    };

    int dstStride[4] =
    {
        image.bytesPerLine(),
        0,
        0,
        0
    };

    sws_scale(
        ctx,
        frame.buffer->plane,
        frame.buffer->stride,
        0,
        frame.height,
        dstData,
        dstStride);

    sws_freeContext(ctx);

    static int frameNo = 0;

    if(frameNo < 5)
    {
        QString file =
                QString("/tmp/bydecoded_%1.png")
                .arg(frameNo,3,10,QLatin1Char('0'));

        image.save(file);

        qDebug() << "Saved decoded:" << file;
    }

    frameNo++;
}
H264Decoder::H264Decoder(QObject *parent)
    : QObject(parent)
{
}

H264Decoder::~H264Decoder()
{
    close();
}

bool H264Decoder::open()
{
    close();

    const AVCodec *codec =
            avcodec_find_decoder(AV_CODEC_ID_H264);

    if (!codec)
        return false;

    m_codecContext = avcodec_alloc_context3(codec);

    if (!m_codecContext)
        return false;

    m_codecContext->flags  |= AV_CODEC_FLAG_LOW_DELAY;
    m_codecContext->flags2 |= AV_CODEC_FLAG2_CHUNKS;
    m_codecContext->skip_loop_filter = AVDISCARD_NONREF;
    m_codecContext->skip_loop_filter = AVDISCARD_ALL;

    if (avcodec_open2(m_codecContext, codec, nullptr) < 0)
    {
        close();
        return false;
    }

    m_frame  = av_frame_alloc();
    m_packet = av_packet_alloc();

    if (!m_frame || !m_packet)
    {
        close();
        return false;
    }

    qDebug() << "H264 decoder initialized.";

    return true;
}

void H264Decoder::close()
{
    if(m_packet)
        av_packet_free(&m_packet);

    if(m_frame)
        av_frame_free(&m_frame);

    if(m_codecContext)
        avcodec_free_context(&m_codecContext);
}

void H264Decoder::decode(const QByteArray &nal,
                         qint64 pts,
                         bool marker)
{
    Q_UNUSED(marker)
    //qDebug()<< "INPUT NAL"<< nal.size()<< "PTS"<< pts;
    const uint8_t *p =
        reinterpret_cast<const uint8_t*>(nal.constData());

   /* qDebug()
        << "DECODE INPUT"
        << "size" << nal.size()
        << "header"
        << QByteArray(reinterpret_cast<const char*>(p),
                      qMin(64, nal.size())).toHex(' ');

    qDebug()
        << "DECODE NAL"
        << (p[4] & 0x1f)
        << "size"
        << nal.size();*/

    if(!m_codecContext)
        return;

    av_packet_unref(m_packet);

    if(av_new_packet(m_packet, nal.size()) < 0)
        return;
    //qDebug() << "Decoder input pts =" << pts;
    memcpy(m_packet->data,
           nal.constData(),
           nal.size());
    ///const uint8_t *p =reinterpret_cast<const uint8_t*>(nal.constData());


    int ret = avcodec_send_packet(m_codecContext,
                                  m_packet);

    /*qDebug()
        << "SEND"

        << m_packet->size;*/

    if(ret < 0)
    {
        char err[256];
        av_strerror(ret, err, sizeof(err));
        qDebug() << "send_packet =" << err;
        return;
    }

    while(true)
    {
        ret = avcodec_receive_frame(m_codecContext,
                                    m_frame);

        if(ret == AVERROR(EAGAIN) ||
           ret == AVERROR_EOF)
        {
            break;
        }

        if(ret < 0)
        {
            char err[256];
            av_strerror(ret, err, sizeof(err));
            qDebug() << "receive_frame =" << err;
            break;
        }
        /*qDebug()
            << "FRAME FORMAT"
            << m_frame->width
            << m_frame->height
            << "crop"
            << m_frame->crop_left
            << m_frame->crop_right
            << m_frame->crop_top
            << m_frame->crop_bottom;*/

        MediaFrame frame;

        frame.width  = m_frame->width;
        frame.height = m_frame->height;
        frame.format = PixelFormat::YUV420P;

        // Artık depacketizer'dan gelen gerçek PTS
        frame.pts = pts;

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
            reinterpret_cast<uint8_t*>(buffer->storage.data()),
            AV_PIX_FMT_YUV420P,
            frame.width,
            frame.height,
            1);

        av_image_copy(
            buffer->plane,
            buffer->stride,
            (const uint8_t**)m_frame->data,
            m_frame->linesize,
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

        //saveDecodedFrame(frame);
        //qDebug() << "FRAME READY";
        emit frameReady(frame);
    }
}
