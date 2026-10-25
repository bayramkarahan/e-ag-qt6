#include "rstpreceiverstream.h"
#include "../decoder/audioopusdecoder.h"
#include "../decoder/h264depacketizer.h"
#include "../decoder/h264decoder.h"
#include "../decoder/CpuVideoDecoder.h"
#include "../decoder/AbstractVideoDecoder.h"
#include "../decoder/videodecoderfactory.h"
#include "../player/audioplayback.h"
#include "../preview/openglvaapizerocopypreview.h"
#include "../preview/openglvideopreview.h"
#include "../preview/videopreview.h"
#include "../preview/AbstractVideoPreview.h"
#include "../rstp-sync/mediaclock.h"
#include "../rstp-sync/videoscheduler.h"
#include "../rstp-sync/audioscheduler.h"

RstpReceiverStream::RstpReceiverStream(QObject *parent)
    : QObject(parent)
{
    m_controlClient = new ControlClient(this);

    connect(m_controlClient,&ControlClient::serverFound,
        this,&RstpReceiverStream::onServerFound);

    connect(m_controlClient,&ControlClient::messageReceived,
        this,&RstpReceiverStream::onControlMessage);

    m_controlClient->start(6001);

  //  m_controlClient->discoverServer();

}
void RstpReceiverStream::onServerFound(
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

void RstpReceiverStream::onControlMessage(
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
            settings.audio.source= AudioSource::None;
            //settings.previewMode = PreviewMode::Remote;
            //settings.videoAddress=QHostAddress("239.10.10.10");
            //settings.videoPort=5004;



            settings.audio.sampleRate=48000;
            settings.audio.channels=2;
            settings.audio.frameSample=960;
            settings.audio.bitrate=20000;
            settings.video.bitrate=4000;

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
bool RstpReceiverStream::open(const StreamSettings &settings)
{
    m_settings = settings;

    return true;
}


bool RstpReceiverStream::start(QString hostIp)
{
    qDebug() << "RUNNING AS RTSP UNICAST RECEIVER";

    m_clock = new MediaClock;

    const bool hasVideo =
        m_settings.video.source != VideoSource::None;

    const bool hasAudio =
        m_settings.audio.source != AudioSource::None;

    m_videoStarted = false;
    m_audioStarted = false;

    // RTSP
    m_rtspSource = new RtspSource(this);

    connect(m_rtspSource,
            &RtspSource::connected,
            this,
            []()
            {
                qDebug() << "RTSP Connected";
            });

    connect(m_rtspSource,
            &RtspSource::disconnected,
            this,
            []()
            {
                qDebug() << "RTSP Disconnected";
            });

    connect(m_rtspSource,
            &RtspSource::errorOccurred,
            this,
            [](const QString &err)
            {
                qCritical() << err;
            });

    // ==================================================
    // VIDEO DECODER + RENDERER
    // ==================================================

    if (hasVideo)
    {
        m_videoDecoder =VideoDecoderFactory::create(this, m_settings);

        if (!m_videoDecoder)
        {
            qCritical() << "No usable video decoder";
            return false;
        }
        m_videoDecoder->setSettings(m_settings);
        m_preview = new VideoPreview;
        m_preview->setSettings(m_settings);

        if (m_settings.previewMode == PreviewMode::NormalPreview)
        {
            m_preview->resize(m_settings.video.width,m_settings.video.height);
            m_preview->show();
        }
        else if (m_settings.previewMode == PreviewMode::MultiPreview)
        {
            m_preview->resize(m_settings.video.width,m_settings.video.height);
            m_preview->show();
        }
        else if (m_settings.previewMode == PreviewMode::FullScreenPreview)
        {
            const QList<QScreen *> screens =
                QGuiApplication::screens();

            if (m_settings.video.screenIndex < 0 ||
                m_settings.video.screenIndex >= screens.size())
            {
                qWarning() << "Invalid screen index:"
                           << m_settings.video.screenIndex;

                return false;
            }

            QScreen *screen =
                screens.at(m_settings.video.screenIndex);

            const QRect geometry = screen->geometry();

            qDebug() << "Fullscreen screen:"
                     << m_settings.video.screenIndex
                     << geometry;

            m_preview->setWindowFlags(
                Qt::FramelessWindowHint |
                Qt::WindowStaysOnTopHint |
                Qt::X11BypassWindowManagerHint);

            m_preview->setGeometry(geometry);
            m_preview->show();
            m_preview->raise();
        }
    }

    // ==================================================
    // AUDIO DECODER + PLAYBACK
    // ==================================================

    if (hasAudio)
    {
        m_playback = new AudioPlayback;

        if (!m_playback->open(
                m_settings.audio.sampleRate,
                m_settings.audio.channels))
        {
            qCritical() << "Audio playback open failed";
            return false;
        }

        m_playback->setClock(m_clock);

        m_audioDecoder = new AudioOpusDecoder(this);

        if (!m_audioDecoder->open(
                m_settings.audio.sampleRate,
                m_settings.audio.channels))
        {
            qCritical() << "Audio decoder open failed";
            return false;
        }
    }

    // ==================================================
    // VIDEO CONNECTIONS
    // ==================================================

    if (hasVideo)
    {
        connect(m_rtspSource,
                &RtspSource::videoPacket,
                m_videoDecoder,
                &AbstractVideoDecoder::decode,
                Qt::QueuedConnection);

        if (hasAudio)
        {
            // Audio + Video: scheduler üzerinden oynatma
            m_videoScheduler = new VideoScheduler(m_clock);

            m_videoScheduler->open(m_settings.video);

            connect(m_videoDecoder,
                    &AbstractVideoDecoder::frameReady,
                    m_videoScheduler,
                    &VideoScheduler::enqueue,
                    Qt::QueuedConnection);

            connect(m_videoScheduler,
                    &VideoScheduler::frameReady,
                    m_preview,
                    &VideoPreview::onFrame,
                    Qt::QueuedConnection);

            connect(m_videoScheduler,
                    &VideoScheduler::firstFrameReceived,
                    this,
                    [this](qint64 pts)
                    {
                        if (m_videoStarted)
                            return;

                        m_videoStarted = true;
                        m_firstVideoPts = pts;

                        qDebug()
                            << "RECEIVER VIDEO FIRST PTS ="
                            << pts;

                        tryStartPlayback();
                    });
        }
        else
        {
            // Yalnızca video: doğrudan renderer
            connect(m_videoDecoder,
                    &AbstractVideoDecoder::frameReady,
                    m_preview,
                    &VideoPreview::onFrame,
                    Qt::QueuedConnection);
        }
    }

    // ==================================================
    // AUDIO CONNECTIONS
    // ==================================================

    if (hasAudio)
    {
        connect(m_rtspSource,
                &RtspSource::audioPacket,
                m_audioDecoder,
                &AudioOpusDecoder::decodeRstp,
                Qt::QueuedConnection);

        if (hasVideo)
        {
            // Audio + Video: scheduler üzerinden oynatma
            m_audioScheduler = new AudioScheduler(m_clock);

            m_audioScheduler->open(m_settings.video);

            connect(m_audioDecoder,
                    &AudioOpusDecoder::frameReady,
                    m_audioScheduler,
                    &AudioScheduler::enqueue,
                    Qt::QueuedConnection);

            connect(m_audioScheduler,
                    &AudioScheduler::frameReady,
                    m_playback,
                    &AudioPlayback::onFrame,
                    Qt::QueuedConnection);

            connect(m_audioScheduler,
                    &AudioScheduler::firstFrameReceived,
                    this,
                    [this](qint64 pts)
                    {
                        if (m_audioStarted)
                            return;

                        m_audioStarted = true;
                        m_firstAudioPts = pts;

                        qDebug()
                            << "RECEIVER AUDIO FIRST PTS ="
                            << pts;

                        tryStartPlayback();
                    });
        }
        else
        {
            // Yalnızca ses: doğrudan playback
            connect(m_audioDecoder,
                    &AudioOpusDecoder::frameReady,
                    m_playback,
                    &AudioPlayback::onFrame,
                    Qt::QueuedConnection);
        }
    }

    // ==================================================
    // RTSP START
    // ==================================================

    const QString url =
        "rtsp://" + hostIp + ":8554/stream";

    m_rtspSource->open(url, m_settings);
    m_rtspSource->start();

    return true;
}

RstpReceiverStream::~RstpReceiverStream()
{
  //  qDebug() << "========== ReceiverStream DESTRUCTOR ==========";

    stop();

    if (m_rtspSource)
    {
        //qDebug() << "DETACH RTSP";
        m_rtspSource->setParent(nullptr);
    }

    if (m_videoDecoder)
    {
        //qDebug() << "DETACH VIDEO DECODER";
        m_videoDecoder->setParent(nullptr);
    }

    //qDebug() << "========== ReceiverStream DESTRUCTOR END ==========";
}


void RstpReceiverStream::stop()
{
   /// qDebug() << "========== ReceiverStream STOP ==========";

    // --------------------------------------------------
    // 1. RTSP DURDUR
    // --------------------------------------------------

    if (m_rtspSource)
    {
       /// qDebug() << "1 - RTSP stop";

        m_rtspSource->stop();

        QObject::disconnect(
            m_rtspSource,
            nullptr,
            nullptr,
            nullptr
        );
    }


    // --------------------------------------------------
    // 2. TÜM SIGNAL ZİNCİRİNİ KES
    // --------------------------------------------------

    if (m_videoDecoder)
    {
        QObject::disconnect(
            m_videoDecoder,
            nullptr,
            nullptr,
            nullptr
        );
    }

    if (m_audioDecoder)
    {
        QObject::disconnect(
            m_audioDecoder,
            nullptr,
            nullptr,
            nullptr
        );
    }

    if (m_videoScheduler)
    {
        QObject::disconnect(
            m_videoScheduler,
            nullptr,
            nullptr,
            nullptr
        );
    }

    if (m_audioScheduler)
    {
        QObject::disconnect(
            m_audioScheduler,
            nullptr,
            nullptr,
            nullptr
        );
    }

    ///qDebug() << "2 - Signals disconnected";


    // --------------------------------------------------
    // 3. DECODER'LARI DURDUR
    // --------------------------------------------------

    if (m_videoDecoder)
    {
       /// qDebug() << "3 - Video decoder stop";
        m_videoDecoder->stop();
    }

    if (m_audioDecoder)
    {
       /// qDebug() << "3 - Audio decoder stop";

        // AudioOpusDecoder'da stop varsa:
        // m_audioDecoder->stop();
    }


    // --------------------------------------------------
    // 4. SCHEDULER'LARI DURDUR
    // --------------------------------------------------

    if (m_videoScheduler)
    {
       /// qDebug() << "4 - Video scheduler stop";

        // varsa:
        // m_videoScheduler->stop();
    }

    if (m_audioScheduler)
    {
       /// qDebug() << "4 - Audio scheduler stop";

        // varsa:
        // m_audioScheduler->stop();
    }


    // --------------------------------------------------
    // 5. AUDIO PLAYBACK
    // --------------------------------------------------

    if (m_playback)
    {
      ///  qDebug() << "5 - Audio playback stop";

        // varsa:
        // m_playback->stop();
    }


    // --------------------------------------------------
    // 6. RENDERER
    // --------------------------------------------------

    if (m_preview)
    {
      ///  qDebug() << "6 - Renderer hide";

        m_preview->hide();
    }


    // --------------------------------------------------
    // 7. STATE
    // --------------------------------------------------

    m_videoStarted = false;
    m_audioStarted = false;

    m_firstVideoPts = 0;
    m_firstAudioPts = 0;

    m_playbackSynchronized = false;


  ///  qDebug() << "========== ReceiverStream STOPPED ==========";
}



void RstpReceiverStream::tryStartPlayback()
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
