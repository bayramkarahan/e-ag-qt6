#include "rtspworker.h"

extern "C"
{
#include <libavformat/avformat.h>
}

RtspWorker::RtspWorker(QObject *parent)
    : QObject(parent)
{
}

RtspWorker::~RtspWorker()
{
    stop();
}

void RtspWorker::setUrl(const QString &url)
{
    m_url = url;
}

void RtspWorker::setSettings(const StreamSettings &settings)
{
        m_settings = settings;
        qDebug()<<"RtspWorker::setSettings"<<
                  m_settings.video.width<<
                  m_settings.video.height<<
                  m_settings.video.fps;
}

void RtspWorker::stop()
{
    if (!m_running)
        return;

    m_running = false;

    if (m_formatContext)
    {
        avformat_close_input(&m_formatContext);
        m_formatContext = nullptr;
    }

    emit disconnected();
}

void RtspWorker::process()
{
    if (m_running)
        return;

    m_running = true;

    m_videoStream = -1;
    m_audioStream = -1;

    AVDictionary *options = nullptr;

    av_dict_set(&options, "rtsp_transport", "udp", 0);
    av_dict_set(&options, "stimeout", "5000000", 0);

    qDebug() << "Opening RTSP:" << m_url;

    if (avformat_open_input(&m_formatContext,
                            m_url.toUtf8().constData(),
                            nullptr,
                            &options) < 0)
    {
        av_dict_free(&options);
        emit errorOccurred("Cannot open RTSP stream");
        m_running = false;
        return;
    }

    av_dict_free(&options);

    qDebug() << "RTSP opened";

    if (avformat_find_stream_info(m_formatContext, nullptr) < 0)
    {
        emit errorOccurred("Cannot read stream info");
        avformat_close_input(&m_formatContext);
        m_running = false;
        return;
    }

    qDebug() << "Stream count =" << m_formatContext->nb_streams;

    for(unsigned i=0;i<m_formatContext->nb_streams;i++)
    {
        AVStream *stream = m_formatContext->streams[i];

        qDebug()
                << "Stream"
                << i
                << "type"
                << stream->codecpar->codec_type
                << "codec"
                << stream->codecpar->codec_id;

        if(stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            m_videoStream = i;

        if(stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
            m_audioStream = i;
    }

    qDebug() << "Video stream =" << m_videoStream;
    qDebug() << "Audio stream =" << m_audioStream;

    if (m_videoStream >= 0)
    {
        AVStream *st = m_formatContext->streams[m_videoStream];

        AVRational rate =
                av_guess_frame_rate(m_formatContext,
                                    st,
                                    nullptr);

        m_sourceFps = av_q2d(rate);

        qDebug() << "Source FPS =" << m_sourceFps;
    }
    emit connected();

    AVPacket packet;

    while(m_running)
    {
        av_init_packet(&packet);

        if(av_read_frame(m_formatContext,&packet) < 0)
            break;

        if(packet.stream_index == m_videoStream){

            processVideoPacket(packet);
        }

        else if(packet.stream_index == m_audioStream)
            processAudioPacket(packet);

        av_packet_unref(&packet);
    }

    avformat_close_input(&m_formatContext);

    m_formatContext = nullptr;

    m_running = false;

    emit disconnected();
}

void RtspWorker::processVideoPacket(const AVPacket &packet)
{


    AVStream *st = m_formatContext->streams[m_videoStream];

    qint64 pts =
        av_rescale_q(packet.pts,
                     st->time_base,
                     AVRational{1,1000000});

    const uint8_t *buf = packet.data;
    int size = packet.size;

    QVector<int> starts;

    // Bütün start code'ları bul
    for (int i = 0; i < size - 3;)
    {
        if (i + 3 < size &&
            buf[i] == 0 &&
            buf[i + 1] == 0 &&
            buf[i + 2] == 0 &&
            buf[i + 3] == 1)
        {
            starts.append(i);
            i += 4;
        }
        else if (buf[i] == 0 &&
                 buf[i + 1] == 0 &&
                 buf[i + 2] == 1)
        {
            starts.append(i);
            i += 3;
        }
        else
        {
            ++i;
        }
    }

    if (starts.isEmpty())
        return;

    starts.append(size);

    for (int i = 0; i < starts.size() - 1; ++i)
    {
        int begin = starts[i];
        int end   = starts[i + 1];

        QByteArray nal(
            reinterpret_cast<const char*>(buf + begin),
            end - begin);

        const uint8_t *p =
            reinterpret_cast<const uint8_t*>(nal.constData());

        int offset = (p[2] == 1) ? 3 : 4;

        int type = p[offset] & 0x1f;

        /*qDebug()
            << "SEND NAL"
            << type
            << nal.size();
        qDebug() << nal.left(16).toHex(' ');
        qDebug() << nal.right(16).toHex(' ');*/
        // Sadece P frameleri seyrekleştir
       /* if (type == 1 &&
            m_settings.video.fps > 0 &&
            m_sourceFps > m_settings.video.fps)
        {
            int skip = qMax(1,
                            qRound(m_sourceFps /
                                   m_settings.video.fps));

            if ((m_frameCounter++ % skip) != 0)
                return;
        }*/

       emit videoPacket(nal, pts, true);
    }
}


void RtspWorker::processAudioPacket(const AVPacket &packet)
{
    AVStream *st = m_formatContext->streams[m_audioStream];

    QByteArray data(
                reinterpret_cast<const char*>(packet.data),
                packet.size);

    qint64 pts =
            av_rescale_q(packet.pts,
                         st->time_base,
                         AVRational{1,1000000});
   /* qDebug() << "RTSP AUDIO PACKET"
             << "pts =" << packet.pts
             << "time =" << QDateTime::currentMSecsSinceEpoch();*/
    emit audioPacket(data,
                     pts);
}
