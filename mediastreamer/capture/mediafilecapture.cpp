#include "mediafilecapture.h"

MediaFileCapture::MediaFileCapture()
{

}
MediaFileCapture::~MediaFileCapture()
{
    close();
}

bool MediaFileCapture::start()
{
    if(!m_format)
        return false;

    m_running = true;

    return true;
}
void MediaFileCapture::stop()
{
    m_running = false;
}
void MediaFileCapture::release(MediaFrame &frame)
{
    Q_UNUSED(frame)

    // MediaBuffer QSharedPointer kullandığı için
    // ekstra işlem gerekmiyor.
}

void MediaFileCapture::close()
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

    if (m_videoCodec)
    {
        avcodec_free_context(&m_videoCodec);
        m_videoCodec = nullptr;
    }

    if (m_audioCodec)
    {
        avcodec_free_context(&m_audioCodec);
        m_audioCodec = nullptr;
    }

    if (m_format)
    {
        avformat_close_input(&m_format);
        m_format = nullptr;
    }
    if (m_swr)
    {
        swr_free(&m_swr);
        m_swr = nullptr;
    }
    m_videoStream = -1;
    m_audioStream = -1;

    m_videoQueue.clear();
    m_audioQueue.clear();

    m_firstPts = AV_NOPTS_VALUE;
}
bool MediaFileCapture::isFinished() const
{
    return m_finished;
}
bool MediaFileCapture::open(const VideoCaptureSettings &settings)
{
    close();
    m_finished = false;
    m_firstPts = AV_NOPTS_VALUE;
    m_audioStartUs=0;
    m_videoStartUs=0;
    //----------------------------------------------------
    // Dosyayı aç
    //----------------------------------------------------
    m_settings = settings;
    m_loop = settings.loop;

    if(    avformat_open_input(
               &m_format,
               settings.fileName.toUtf8().constData(),
               nullptr,
               nullptr) < 0)
    {
        qWarning() << "Cannot open file:" << settings.fileName;
        return false;
    }

    if(avformat_find_stream_info(m_format, nullptr) < 0)
    {
        qWarning() << "Cannot read stream info.";
        close();
        return false;
    }

    //----------------------------------------------------
    // Streamleri bul
    //----------------------------------------------------

    m_videoStream = -1;
    m_audioStream = -1;

    for(unsigned i = 0; i < m_format->nb_streams; ++i)
    {
        AVCodecParameters *par =
                m_format->streams[i]->codecpar;

        if(par->codec_type == AVMEDIA_TYPE_VIDEO &&
           m_videoStream < 0)
        {
            m_videoStream = i;
        }

        if(par->codec_type == AVMEDIA_TYPE_AUDIO &&
           m_audioStream < 0)
        {
            m_audioStream = i;
        }
    }

    //----------------------------------------------------
    // Video decoder
    //----------------------------------------------------

    if(m_videoStream >= 0)
    {
        AVCodecParameters *par =
                m_format->streams[m_videoStream]->codecpar;

        const AVCodec *codec =
                avcodec_find_decoder(par->codec_id);
        qDebug()
            << "Codec:"
            << codec->name
            << codec->id;
        qDebug()
            << "Video codec id ="
            << m_format->streams[m_videoStream]->codecpar->codec_id;

        if(codec)
        {
            m_videoCodec =
                    avcodec_alloc_context3(codec);

            avcodec_parameters_to_context(
                        m_videoCodec,
                        par);
            qDebug()
                << "REAL VIDEO"
                << m_videoCodec->width
                << m_videoCodec->height;
            qDebug()
                << "seting VIDEO"
                << m_settings.width
                << m_settings.height;

            if(avcodec_open2(m_videoCodec,
                             codec,
                             nullptr) < 0)
            {
                qWarning() << "Cannot open video decoder.";
                close();
                return false;
            }

        }
    }

    //----------------------------------------------------
    // Audio decoder
    //----------------------------------------------------

    if(m_audioStream >= 0)
    {
        AVCodecParameters *par =
                m_format->streams[m_audioStream]->codecpar;

        const AVCodec *codec =
                avcodec_find_decoder(par->codec_id);

        if(codec)
        {
            m_audioCodec =
                    avcodec_alloc_context3(codec);

            avcodec_parameters_to_context(
                        m_audioCodec,
                        par);

            if(avcodec_open2(m_audioCodec,
                             codec,
                             nullptr) < 0)
            {
                qWarning() << "Cannot open audio decoder.";
                close();
                return false;
            }
            AVChannelLayout outLayout = AV_CHANNEL_LAYOUT_STEREO;

            swr_alloc_set_opts2(
                    &m_swr,
                    &outLayout,
                    AV_SAMPLE_FMT_S16,
                    48000,

                    &m_audioCodec->ch_layout,
                    m_audioCodec->sample_fmt,
                    m_audioCodec->sample_rate,

                    0,
                    nullptr);

            if (!m_swr || swr_init(m_swr) < 0)
            {
                qDebug() << "Cannot init swr";
                return false;
            }
        }
    }

    //----------------------------------------------------
    // Packet / Frame
    //----------------------------------------------------

    m_packet = av_packet_alloc();
    m_frame  = av_frame_alloc();

    if(!m_packet || !m_frame)
    {
        close();
        return false;
    }

    qDebug() << "Media file opened.";
    qDebug() << "Video stream =" << m_videoStream;
    qDebug() << "Audio stream =" << m_audioStream;

    return true;
}
void MediaFileCapture::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
}

bool MediaFileCapture::grab(MediaFrame &frame)
{
    QMutexLocker locker(&m_videoMutex);

    while (m_videoQueue.isEmpty())
        m_notEmptyVideo.wait(&m_videoMutex);

    frame = m_videoQueue.dequeue();

    m_notFullVideo.wakeOne();

    return true;
}

bool MediaFileCapture::grabAudio(AudioFrame &frame)
{
    QMutexLocker locker(&m_audioMutex);

    while (m_audioQueue.isEmpty())
        m_notEmptyAudio.wait(&m_audioMutex);

    frame = m_audioQueue.dequeue();

    m_notFullAudio.wakeOne();

    return true;
}

bool MediaFileCapture::readNextPacket()
{
    int ret = av_read_frame(m_format, m_packet);

    if (ret < 0)
        return false;

    if (m_packet->stream_index == m_videoStream)
    {
        decodeVideoPacket();
    }
    else if (m_packet->stream_index == m_audioStream)
    {
        decodeAudioPacket();
    }

    return true;
}
/*bool MediaFileCapture::readNextPacket()
{
    while (true)
    {
        int ret = av_read_frame(m_format, m_packet);

        if (ret < 0)
            return false;

        qDebug()
                << "READ stream="
                << m_packet->stream_index;

        if (m_packet->stream_index == m_videoStream)
        {
            decodeVideoPacket();

            if (!m_videoQueue.isEmpty())
            {
                qDebug()
                        << "QUEUE V="
                        << m_videoQueue.size()
                        << "A="
                        << m_audioQueue.size();

                return true;
            }
        }
        else if (m_packet->stream_index == m_audioStream)
        {
            decodeAudioPacket();

            if (!m_audioQueue.isEmpty())
            {
                qDebug()
                        << "QUEUE V="
                        << m_videoQueue.size()
                        << "A="
                        << m_audioQueue.size();

                return true;
            }
        }
    }
}*/
/*
bool MediaFileCapture::readNextPacket()
{
    while (av_read_frame(m_format, m_packet) >= 0)
    {
        qDebug()
            << "READ PACKET stream="
            << m_packet->stream_index;

        if (m_packet->stream_index == m_videoStream)
            decodeVideoPacket();
        else if (m_packet->stream_index == m_audioStream)
            decodeAudioPacket();
        else
            av_packet_unref(m_packet);

        qDebug()
            << "QUEUE video="
            << m_videoQueue.size()
            << "audio="
            << m_audioQueue.size();

        if (!m_videoQueue.isEmpty() ||
            !m_audioQueue.isEmpty())
            return true;
    }


}*/
/*
bool MediaFileCapture::readNextPacket()
{
    while (true)
    {
        // İki queue da doluysa artık okumaya gerek yok.
        if (!m_videoQueue.isEmpty() &&
            !m_audioQueue.isEmpty())
        {
            return true;
        }

        int ret = av_read_frame(m_format, m_packet);

        if (ret < 0)
            break;

        if (m_packet->stream_index == m_videoStream)
        {
            decodeVideoPacket();
        }
        else if (m_packet->stream_index == m_audioStream)
        {
            decodeAudioPacket();
        }
        else
        {
            av_packet_unref(m_packet);
        }
    }

    if (m_loop)
    {
        if (restart())
            return readNextPacket();
    }

    return !m_videoQueue.isEmpty() ||
           !m_audioQueue.isEmpty();
}

*/

void MediaFileCapture::decodeVideoPacket()
{
    int ret = avcodec_send_packet(m_videoCodec, m_packet);

    av_packet_unref(m_packet);

    if (ret < 0)
        return;

    while (true)
    {
        ret = avcodec_receive_frame(m_videoCodec, m_frame);

        ///qDebug() << "receive_packet =" << ret;

        if (ret == AVERROR(EAGAIN))
        {
            ///qDebug() << "decoder EAGAIN";
            break;
        }

        if (ret == AVERROR_EOF)
        {
           // qDebug() << "decoder EOF";
            break;
        }

        if (ret < 0)
        {
            qDebug() << "decoder error =" << ret;
            break;
        }

        MediaFrame frame;

        auto buffer = QSharedPointer<MediaBuffer>::create();

        AVPixelFormat fmt =
                static_cast<AVPixelFormat>(m_frame->format);

        int size =
                av_image_get_buffer_size(
                    fmt,
                    m_frame->width,
                    m_frame->height,
                    1);

        if (size <= 0)
            continue;

        buffer->storage.resize(size);

        av_image_fill_arrays(
                    buffer->plane,
                    buffer->stride,
                    reinterpret_cast<uint8_t*>(buffer->storage.data()),
                    fmt,
                    m_frame->width,
                    m_frame->height,
                    1);

        av_image_copy(
                    buffer->plane,
                    buffer->stride,
                    const_cast<const uint8_t **>(m_frame->data),
                    m_frame->linesize,
                    fmt,
                    m_frame->width,
                    m_frame->height);

        buffer->planeCount =
                av_pix_fmt_count_planes(fmt);

        frame.width  = m_frame->width;
        frame.height = m_frame->height;

        switch (fmt)
        {
        case AV_PIX_FMT_YUV420P:
            frame.format = PixelFormat::YUV420P;
            break;

        case AV_PIX_FMT_NV12:
            frame.format = PixelFormat::NV12;
            break;

        case AV_PIX_FMT_RGBA:
            frame.format = PixelFormat::RGBA8888;
            break;

        case AV_PIX_FMT_RGB24:
            frame.format = PixelFormat::RGB24;
            break;

        default:
            continue;
        }

        frame.buffer = buffer;

        AVRational tb =
                m_format->streams[m_videoStream]->time_base;

        frame.pts = av_rescale_q(
                m_frame->pts,
                tb,
                AVRational{1,1000000});

        if (m_globalStartPts == AV_NOPTS_VALUE)
            m_globalStartPts = frame.pts;

        frame.pts -= m_globalStartPts;
        /*qDebug()
            << "VIDEO ENQUEUE"
            << frame.width
            << frame.height
            << frame.pts;*/
        {
            QMutexLocker locker(&m_videoMutex);

            while (m_videoQueue.size() >= MAX_VIDEO_QUEUE)
                m_notFullVideo.wait(&m_videoMutex);

            m_videoQueue.enqueue(frame);

            m_notEmptyVideo.wakeOne();
        }
    }
}

void MediaFileCapture::decodeAudioPacket()
{
    int ret = avcodec_send_packet(m_audioCodec, m_packet);

    av_packet_unref(m_packet);

    if (ret < 0)
        return;

    while (true)
    {
        ret = avcodec_receive_frame(m_audioCodec, m_frame);

       /// qDebug() << "audio receive_frame =" << ret;

        if (ret == AVERROR(EAGAIN))
        {
           /// qDebug() << "audio decoder EAGAIN";
            break;
        }

        if (ret == AVERROR_EOF)
        {
            qDebug() << "audio decoder EOF";
            break;
        }

        if (ret < 0)
        {
            qDebug() << "audio decoder error =" << ret;
            break;
        }
       /* qDebug()
            << "sample format ="
            << av_get_sample_fmt_name(
                   (AVSampleFormat)m_frame->format);*/
      /*  qDebug()
            << "Audio:"
            << "format =" << av_get_sample_fmt_name(
                                 static_cast<AVSampleFormat>(m_frame->format))
            << "rate =" << m_frame->sample_rate
            << "channels =" << m_frame->ch_layout.nb_channels
            << "samples =" << m_frame->nb_samples;*/
        AudioFrame frame;

        frame.sampleRate = m_frame->sample_rate;
        frame.channels   = m_frame->ch_layout.nb_channels;

        // Şimdilik planar / packed ayırmadan ham veriyi kopyalayacağız.
        // AudioProcessor gelince burada dönüşüm yapılacak.


        frame.sampleRate = 48000;
        frame.channels   = 2;
        frame.sampleSize = 16;

        int outSamples = av_rescale_rnd(
                swr_get_delay(m_swr,
                              m_frame->sample_rate)
                    + m_frame->nb_samples,
                48000,
                m_frame->sample_rate,
                AV_ROUND_UP);

        frame.data.resize(
                outSamples *
                frame.channels *
                sizeof(qint16));

        uint8_t *dst[] =
        {
            reinterpret_cast<uint8_t*>(frame.data.data())
        };

        int converted = swr_convert(
                m_swr,
                dst,
                outSamples,
                (const uint8_t**)m_frame->data,
                m_frame->nb_samples);

        if (converted <= 0)
            continue;

        frame.data.resize(
                converted *
                frame.channels *
                sizeof(qint16));

        AVRational tb =
                m_format->streams[m_audioStream]->time_base;

        frame.pts = av_rescale_q(
                m_frame->pts,
                tb,
                AVRational{1,1000000});

        if (m_globalStartPts == AV_NOPTS_VALUE)
            m_globalStartPts = frame.pts;

        frame.pts -= m_globalStartPts;

       /* qDebug()
                << "AUDIO ENQUEUE"
                << frame.sampleRate
                << frame.channels
                << frame.data.size()
                << frame.pts;*/
        {
            QMutexLocker locker(&m_audioMutex);

            while (m_audioQueue.size() >= MAX_AUDIO_QUEUE)
                m_notFullAudio.wait(&m_audioMutex);

            m_audioQueue.enqueue(frame);

            m_notEmptyAudio.wakeOne();
        }
        //m_audioQueue.enqueue(frame);
    }
}

bool MediaFileCapture::grabNext(MediaFrame &video,
                                AudioFrame &audio,
                                bool &isVideo)
{
    // Her çağrıda bir paket daha oku
    readNextPacket();

    bool hasVideo = !m_videoQueue.isEmpty();
    bool hasAudio = !m_audioQueue.isEmpty();

    if (!hasVideo && !hasAudio)
        return false;

    if (hasVideo &&
        (!hasAudio ||
         m_videoQueue.head().pts <=
         m_audioQueue.head().pts))
    {
        video = m_videoQueue.dequeue();
        isVideo = true;
        return true;
    }

    audio = m_audioQueue.dequeue();
    isVideo = false;
    return true;
}
/*
bool MediaFileCapture::grabNext(MediaFrame &video,
                                AudioFrame &audio,
                                bool &isVideo)
{
    while (true)
    {
        //
        // Queue boşsa yeni packet oku
        //
        if (m_videoQueue.isEmpty() &&
            m_audioQueue.isEmpty())
        {
            if (!readNextPacket())
                return false;
        }

        bool hasVideo = !m_videoQueue.isEmpty();
        bool hasAudio = !m_audioQueue.isEmpty();

        if (!hasVideo && !hasAudio)
            continue;

        //
        // İkisi de varsa küçük PTS seç
        //
        if (hasVideo && hasAudio)
        {
            qDebug()
                    << "COMPARE"
                    << "V=" << m_videoQueue.head().pts
                    << "A=" << m_audioQueue.head().pts;

            if (m_videoQueue.head().pts <=
                m_audioQueue.head().pts)
            {
                video = m_videoQueue.dequeue();
                isVideo = true;
                return true;
            }

            audio = m_audioQueue.dequeue();
            isVideo = false;
            return true;
        }

        //
        // Sadece video
        //
        if (hasVideo)
        {
            video = m_videoQueue.dequeue();
            isVideo = true;
            return true;
        }

        //
        // Sadece audio
        //
        audio = m_audioQueue.dequeue();
        isVideo = false;
        return true;
    }
}
*/
bool MediaFileCapture::hasVideoFrame() const
{
    QMutexLocker locker(&m_videoMutex);

    return !m_videoQueue.isEmpty();
}
bool MediaFileCapture::hasAudioFrame() const
{
    QMutexLocker locker(&m_audioMutex);

    return !m_audioQueue.isEmpty();
}

qint64 MediaFileCapture::videoPts() const
{
    QMutexLocker locker(&m_videoMutex);

    if (m_videoQueue.isEmpty())
        return LLONG_MAX;

    return m_videoQueue.head().pts;
}
qint64 MediaFileCapture::audioPts() const
{
    QMutexLocker locker(&m_audioMutex);

    if (m_audioQueue.isEmpty())
        return LLONG_MAX;

    return m_audioQueue.head().pts;
}
void MediaFileCapture::setLoop(bool enabled)
{
    m_loop = enabled;
}

bool MediaFileCapture::restart()
{
    if(!m_format)
        return false;

    avcodec_flush_buffers(m_videoCodec);

    if(m_audioCodec)
        avcodec_flush_buffers(m_audioCodec);

    if(av_seek_frame(
            m_format,
            -1,
            0,
            AVSEEK_FLAG_BACKWARD) < 0)
    {
        return false;
    }

    while(!m_videoQueue.isEmpty())
        m_videoQueue.dequeue();

    while(!m_audioQueue.isEmpty())
        m_audioQueue.dequeue();

    m_firstVideoPts = AV_NOPTS_VALUE;
    m_firstAudioPts = AV_NOPTS_VALUE;

    m_videoStartUs = 0;
    m_audioStartUs = 0;

    return true;
}

void MediaFileCapture::pause(bool pause)
{
    m_paused = pause;
}

bool MediaFileCapture::isPaused() const
{
    return m_paused;
}

qint64 MediaFileCapture::duration() const
{
    if(!m_format)
        return 0;

    if(m_format->duration == AV_NOPTS_VALUE)
        return 0;

    return m_format->duration / 1000;
}

qint64 MediaFileCapture::position() const
{
    if(m_firstVideoPts == AV_NOPTS_VALUE)
        return 0;

    return (m_firstVideoPts / 1000);
}

bool MediaFileCapture::seek(qint64 ms)
{
    if(!m_format)
        return false;

    qint64 ts =
        av_rescale_q(
            ms,
            AVRational{1,1000},
            AV_TIME_BASE_Q);

    if(av_seek_frame(
            m_format,
            -1,
            ts,
            AVSEEK_FLAG_BACKWARD) < 0)
    {
        return false;
    }

    avcodec_flush_buffers(m_videoCodec);

    if(m_audioCodec)
        avcodec_flush_buffers(m_audioCodec);

    m_videoQueue.clear();
    m_audioQueue.clear();

    m_firstVideoPts = AV_NOPTS_VALUE;
    m_firstAudioPts = AV_NOPTS_VALUE;

    return true;
}

