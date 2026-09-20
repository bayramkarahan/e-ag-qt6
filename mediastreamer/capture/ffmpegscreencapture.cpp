#include "ffmpegscreencapture.h"

#include <QDebug>

extern "C"
{
#include <libavutil/dict.h>
}

FFmpegScreenCapture::FFmpegScreenCapture()
{
    avdevice_register_all();
}

FFmpegScreenCapture::~FFmpegScreenCapture()
{
    close();
}

bool FFmpegScreenCapture::open(const VideoCaptureSettings &settings)
{
    close();

    m_settings = settings;


    const AVInputFormat *input =
        av_find_input_format("x11grab");

    if(!input)
    {
        qWarning() << "x11grab input not found.";
        return false;
    }

    AVDictionary *options = nullptr;

    QString videoSize =
            QString("%1x%2")
                .arg(settings.width)
                .arg(settings.height);

    av_dict_set(&options,
                "video_size",
                videoSize.toUtf8().constData(),
                0);

    av_dict_set(&options,
                "framerate",
                QByteArray::number(settings.fps).constData(),
                0);

    /*
        İlk monitör
    */
    const char *display = ":0.0";

    int ret =
        avformat_open_input(
            &m_format,
            display,
            input,
            &options);

    av_dict_free(&options);

    if(ret < 0)
    {
        qWarning() << "Cannot open x11grab input.";
        return false;
    }

    ret = avformat_find_stream_info(
                m_format,
                nullptr);

    if(ret < 0)
    {
        qWarning() << "Cannot read stream info.";
        close();
        return false;
    }

    m_streamIndex = -1;

    for(unsigned i = 0;
        i < m_format->nb_streams;
        ++i)
    {
        if(m_format->streams[i]->codecpar->codec_type
                == AVMEDIA_TYPE_VIDEO)
        {
            m_streamIndex = i;
            break;
        }
    }

    if(m_streamIndex < 0)
    {
        qWarning() << "Video stream not found.";
        close();
        return false;
    }

    AVCodecParameters *codecpar =
            m_format
            ->streams[m_streamIndex]
            ->codecpar;

    const AVCodec *decoder =
            avcodec_find_decoder(
                codecpar->codec_id);

    if(!decoder)
    {
        qWarning() << "Decoder not found.";
        close();
        return false;
    }

    m_codec =
        avcodec_alloc_context3(decoder);

    if(!m_codec)
    {
        close();
        return false;
    }

    avcodec_parameters_to_context(
                m_codec,
                codecpar);

    ret =
        avcodec_open2(
            m_codec,
            decoder,
            nullptr);
   /* qDebug() << "Stream index =" << m_streamIndex;
    qDebug() << "Codec =" << decoder->name;
    qDebug() << "Width =" << m_codec->width;
    qDebug() << "Height =" << m_codec->height;

    qDebug() << "FFmpeg screen opened.";*/
    if(ret < 0)
    {
        qWarning() << "Cannot open decoder.";
        close();
        return false;
    }

    m_packet = av_packet_alloc();
    m_frame  = av_frame_alloc();

    qDebug()
        << "FFmpeg screen opened:"
        << codecpar->width
        << "x"
        << codecpar->height
        << settings.fps
        << "fps";

    return true;
}

bool FFmpegScreenCapture::start()
{
    if(!m_codec)
        return false;

    m_running = true;

    return true;
}

void FFmpegScreenCapture::stop()
{
    m_running = false;
}

void FFmpegScreenCapture::close()
{
    stop();

    if(m_packet)
    {
        av_packet_free(&m_packet);
        m_packet = nullptr;
    }

    if(m_frame)
    {
        av_frame_free(&m_frame);
        m_frame = nullptr;
    }

    if(m_codec)
    {
        avcodec_free_context(&m_codec);
        m_codec = nullptr;
    }

    if(m_format)
    {
        avformat_close_input(&m_format);
        m_format = nullptr;
    }

    m_streamIndex = -1;
}

void FFmpegScreenCapture::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
}

void FFmpegScreenCapture::release(MediaFrame &frame)
{
    Q_UNUSED(frame)
}

bool FFmpegScreenCapture::grab(MediaFrame &frame)
{
    if(!m_running || !m_format || !m_codec)
        return false;

    while(av_read_frame(m_format, m_packet) >= 0)
    {
        if(m_packet->stream_index != m_streamIndex)
        {
            av_packet_unref(m_packet);
            continue;
        }

        int ret = avcodec_send_packet(m_codec, m_packet);
        av_packet_unref(m_packet);

        if(ret < 0)
            continue;

        ret = avcodec_receive_frame(m_codec, m_frame);

        if(ret == AVERROR(EAGAIN))
            continue;

        if(ret == AVERROR_EOF)
            return false;

        if(ret < 0)
            continue;

        SwsContext *ctx =
            sws_getContext(
                m_frame->width,
                m_frame->height,
                AV_PIX_FMT_BGR0,
                m_frame->width,
                m_frame->height,
                AV_PIX_FMT_RGBA,
                SWS_FAST_BILINEAR,
                nullptr,
                nullptr,
                nullptr);

        if(!ctx)
            return false;

        auto buffer = QSharedPointer<MediaBuffer>(new MediaBuffer);

        int size = av_image_get_buffer_size(
                    AV_PIX_FMT_RGBA,
                    m_frame->width,
                    m_frame->height,
                    1);

        buffer->storage.resize(size);

        av_image_fill_arrays(
                    buffer->plane,
                    buffer->stride,
                    reinterpret_cast<uint8_t*>(buffer->storage.data()),
                    AV_PIX_FMT_RGBA,
                    m_frame->width,
                    m_frame->height,
                    1);

        const uint8_t *srcData[4] =
        {
            m_frame->data[0],
            nullptr,
            nullptr,
            nullptr
        };

        int srcStride[4] =
        {
            m_frame->linesize[0],
            0,
            0,
            0
        };

        sws_scale(
                    ctx,
                    srcData,
                    srcStride,
                    0,
                    m_frame->height,
                    buffer->plane,
                    buffer->stride);

        sws_freeContext(ctx);

        buffer->planeCount = 1;

        frame.width  = m_frame->width;
        frame.height = m_frame->height;
        frame.format = PixelFormat::RGBA8888;
        frame.buffer = buffer;

        frame.pts = m_masterTime ? m_masterTime->nowUs() : 0;

        return true;
    }

    return false;
}
