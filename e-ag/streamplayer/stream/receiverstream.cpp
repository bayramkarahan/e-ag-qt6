#include "receiverstream.h"

#include "../decoder/audioopusdecoder.h"
#include "../decoder/h264depacketizer.h"

#include "../decoder/h264decoder.h"
#include "../decoder/CpuVideoDecoder.h"

#include "../decoder/AbstractVideoDecoder.h"
#include "../decoder/videodecoderfactory.h"

#include "../player/audioplayback.h"

#include "../renderer/openglvaapizerocopyrenderer.h"
#include "../renderer/openglvideorenderer.h"
#include "../renderer/videorenderer.h"
#include "../renderer/AbstractVideoRenderer.h"


#include "../sync/mastertime.h"
#include "../sync/mediaclock.h"
#include "../sync/videoscheduler.h"
#include "../sync/audioscheduler.h"

ReceiverStream::ReceiverStream(QObject *parent)
    : QObject(parent)
{
    m_controlClient = new ControlClient(this);

    connect(m_controlClient,&ControlClient::serverFound,
        this,&ReceiverStream::onServerFound);

    connect(m_controlClient,&ControlClient::messageReceived,
        this,&ReceiverStream::onControlMessage);

    m_controlClient->start(6001);

   // m_controlClient->discoverServer();

}
void ReceiverStream::onServerFound(
    const QHostAddress &address,
    quint16 port)
{
    qInfo()
        << "Sender found:"
        << address.toString()
        << port;


    /*
     * Artık Server'ın IP'sini biliyoruz.
     *
     * Discovery otomatik olarak durdu.
     */


    /*
     * Örneğin hemen stream bilgilerini iste.
     */


    m_controlClient->sendToServer(
        ControlMessageType::GetStreamInfo);
}

void ReceiverStream::onControlMessage(
    const ControlMessage &message)
{
    switch (message.type)
    {
        case ControlMessageType::StreamInfo:
        {
            qInfo()
                << "STREAM_INFO received";

            /*
             * message.payload içinden
             * StreamSettings okunacak.
             */
            m_controlClient->sendToServer(
                ControlMessageType::Ping);
            //m_controlClient->sendToServer(ControlMessageType::StartStreamFFmpegScreenCaptureVideo);

            break;
        }

        case ControlMessageType::StartStream:
        {
            qInfo()
                << "StartStream received";
            StreamSettings settings;
            settings.mode=StreamMode::Receiver;
            //settings.video.source = CaptureSource::Camera;
            //settings.video.fileName="selcuk.mp4";
           /* if (!CommandLineParser::parse(app, settings))
                return -1;*/


            //settings.video.device="/dev/video2";
            settings.captureMode = CaptureMode::Video;
            //settings.previewMode = PreviewMode::Remote;
            //settings.videoAddress=QHostAddress("239.10.10.10");
            //settings.videoPort=5004;



            settings.audioSampleRate=48000;
            settings.audioChannels=2;
            settings.audioFrameSample=960;
            settings.audioBitrate=20000;
            settings.videoBitrate=4000;

            settings.audioAddress=settings.videoAddress;
            settings.audioPort=settings.videoPort+2;
            settings.video.screenIndex = 0;
            //settings.video.fps    = 5;
            //settings.video.width  = 160;
            //settings.video.height = 90;
            //settings.video.timeMode = TimeMode::MasterClock;
            if (!open(settings)) return;
            this->start(m_controlClient->serverAddress().toString());
            break;
        }
        case ControlMessageType::Pong:
        {
            qInfo()
                << "PONG received";

            break;
        }


        default:
            break;
    }
}
bool ReceiverStream::open(const StreamSettings &settings)
{
    m_settings = settings;

    return true;
}
bool ReceiverStream::start(QString hostIp)
{
    qDebug() << "RUNNING AS RECEIVER";

    m_clock = new MediaClock;


    // RTSP
    m_rtspSource = new RtspSource(this);

    connect(m_rtspSource,
            &RtspSource::connected,
            []()
            {
                qDebug() << "RTSP Connected";
            });

    connect(m_rtspSource,
            &RtspSource::disconnected,
            []()
            {
                qDebug() << "RTSP Disconnected";
            });

    connect(m_rtspSource,
            &RtspSource::errorOccurred,
            [](const QString &err)
            {
                qCritical() << err;
            });

    ///video
    if(m_settings.captureMode==CaptureMode::Video||
            m_settings.captureMode==CaptureMode::AudioVideo)
    {
        m_videoDecoder = VideoDecoderFactory::create(this);
        if(!m_videoDecoder->start())
            return -1;
        m_videoDecoder->setSettings(m_settings);

        if (m_videoDecoder->type() ==AbstractVideoDecoder::Type::VAAPI)
        {
            qDebug() << "Renderer: VAAPI";
            m_openglrenderer =new OpenGLVaapiZeroCopyRenderer;
        }
        else
        {
            qDebug() << "Renderer: CPU";
            m_openglrenderer =new OpenGLVideoRenderer;
            //m_openglrenderer =new VideoRenderer;
        }

      /* if (m_settings.previewMode != PreviewMode::None)
        {
            m_openglrenderer->resize(m_settings.video.width, m_settings.video.height);
            m_openglrenderer->show();
        }*/
    }

    if(m_settings.captureMode==CaptureMode::Video)
    {
        connect(m_rtspSource,&RtspSource::videoPacket,
                m_videoDecoder,&AbstractVideoDecoder::decode,
                Qt::QueuedConnection);

        connect(m_videoDecoder,&AbstractVideoDecoder::frameReady,
                m_openglrenderer,&AbstractVideoRenderer::onFrame,
                Qt::QueuedConnection);
    }

    if(m_settings.captureMode==CaptureMode::AudioVideo)
    {
        connect(m_rtspSource,&RtspSource::videoPacket,
                m_videoDecoder,&AbstractVideoDecoder::decode,
                Qt::QueuedConnection);

        m_videoScheduler = new VideoScheduler(m_clock);
        m_videoScheduler->open(m_settings.video);
        connect(m_videoScheduler,&VideoScheduler::firstFrameReceived,this,[this](qint64 pts)
        {
            if (m_videoStarted) return;
            m_videoStarted = true;
            m_firstVideoPts = pts;
            qDebug() << "RECEIVER VIDEO FIRST PTS =" << pts;
            tryStartPlayback();
        });

        connect(m_videoDecoder,&AbstractVideoDecoder::frameReady,
                m_videoScheduler,&VideoScheduler::enqueue,
                Qt::QueuedConnection);

        connect(m_videoScheduler,&VideoScheduler::frameReady,
                m_openglrenderer,&AbstractVideoRenderer::onFrame,
                Qt::QueuedConnection);
    }
    ///ses
    if(m_settings.captureMode==CaptureMode::Audio||
            m_settings.captureMode==CaptureMode::AudioVideo)
    {
        m_playback = new AudioPlayback;
        if (!m_playback->open(m_settings.audioSampleRate,
                              m_settings.audioChannels))
            return false;
        m_playback->setClock(m_clock);
        m_audioDecoder = new AudioOpusDecoder(this);

        if (!m_audioDecoder->open(m_settings.audioSampleRate,
                                  m_settings.audioChannels))
            return false;
    }

    if(m_settings.captureMode==CaptureMode::Audio)
    {
        connect(m_rtspSource,&RtspSource::audioPacket,
                m_audioDecoder,&AudioOpusDecoder::decode,
                Qt::QueuedConnection);

        connect(m_audioDecoder,&AudioOpusDecoder::frameReady,
                m_playback,&AudioPlayback::onFrame,
                Qt::QueuedConnection);
    }
    if(m_settings.captureMode==CaptureMode::AudioVideo)
    {
        m_audioScheduler = new AudioScheduler(m_clock);
        m_audioScheduler->open(m_settings.video);
        connect(m_audioScheduler,&AudioScheduler::firstFrameReceived,this,[this](qint64 pts)
        {
            if (m_audioStarted) return;
            m_audioStarted = true;
            m_firstAudioPts = pts;
            qDebug() << "RECEIVER AUDIO FIRST PTS =" << pts;
            tryStartPlayback();
        });

        connect(m_rtspSource,&RtspSource::audioPacket,
                m_audioDecoder,&AudioOpusDecoder::decode,
                Qt::QueuedConnection);

        connect(m_audioDecoder,&AudioOpusDecoder::frameReady,
                m_audioScheduler,&AudioScheduler::enqueue,
                Qt::QueuedConnection);

        connect(m_audioScheduler,&AudioScheduler::frameReady,
                m_playback,&AudioPlayback::onFrame,
                Qt::QueuedConnection);
    }

    QString url="rtsp://"+hostIp+":8554/stream";

    m_rtspSource->open(url,m_settings);
    m_rtspSource->start();

    return true;
}


void ReceiverStream::stop()
{

}
void ReceiverStream::tryStartPlayback()
{
    if (!m_audioStarted && !m_videoStarted)
        return;

    if (!m_clock->isStarted())
    {
        qint64 startPts;

        if (m_audioStarted && m_videoStarted)
            startPts = qMin(m_firstAudioPts, m_firstVideoPts);
        else if (m_audioStarted)
            startPts = m_firstAudioPts;
        else
            startPts = m_firstVideoPts;

        qDebug() << "START MASTER CLOCK =" << startPts;

        m_clock->start(startPts);

        return;
    }

    // İkinci stream yeni geldiyse bir kez yeniden senkronize et
    if (m_audioStarted &&
        m_videoStarted &&
        !m_playbackSynchronized)
    {
        qint64 startPts =
            qMin(m_firstAudioPts,
                 m_firstVideoPts);

        qDebug() << "RESYNC MASTER CLOCK ="
                 << startPts;

        m_clock->restart(startPts);

        m_playbackSynchronized = true;
    }
}
