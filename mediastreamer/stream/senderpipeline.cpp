#include "senderpipeline.h"
#include "../capture/waylandscreencast.h"

SenderPipeline::SenderPipeline(QObject *parent)
    : QObject(parent)
{
    m_controlServer =
        new ControlServer(this);


    connect(
        m_controlServer,
        &ControlServer::messageReceived,
        this,
        &SenderPipeline::onControlMessage);


    connect(
        m_controlServer,
        &ControlServer::errorOccurred,
        this,
        [](const QString &error)
        {
            qWarning()
                << "Control server:"
                << error;
        });


    if (!m_controlServer->start(6000))
    {
        qWarning()
            << "Control server failed";

        return;
    }


    WaylandScreenCast *screenCast =
        new WaylandScreenCast(this);

    connect(screenCast,
            &WaylandScreenCast::sessionCreated,
            this,
            [screenCast]()
    {
        screenCast->selectSources(
            true,   // monitor
            false,  // window
            false   // cursor
        );
    });

    connect(screenCast,
            &WaylandScreenCast::sourcesSelected,
            this,
            [screenCast]()
    {
        screenCast->start();
    });

    connect(screenCast,
            &WaylandScreenCast::started,
            this,
            [screenCast]()
    {
        qDebug()
            << "Node:"
            << screenCast->nodeId();

        screenCast->openPipeWireRemote();
    });

    connect(screenCast,
            &WaylandScreenCast::pipeWireRemoteOpened,
            this,
            [](int fd)
    {
        qDebug()
            << "PipeWire FD:"
            << fd;
    });

    connect(screenCast,
            &WaylandScreenCast::ready,
            this,
            [screenCast]()
    {
        qDebug()
            << "Screen capture ready:"
            << "node =" << screenCast->nodeId()
            << "fd =" << screenCast->pipeWireFd();
    });

    connect(screenCast,
            &WaylandScreenCast::error,
            this,
            [](const QString &error)
    {
        qWarning()
            << "ScreenCast error:"
            << error;
    });

    screenCast->createSession();


}
void SenderPipeline::onControlMessage(
    const QHostAddress &address,
    quint16 port,
    const ControlMessage &message)
{

    qDebug()
        << "Control message from"
        << address.toString()
        << port
        << "type:"
        << static_cast<quint16>(
               message.type);


    switch (message.type)
    {
       case ControlMessageType::GetStreamInfo:
       {
           qDebug() << "Client requested stream info";

           // StreamSettings oluştur
           // QDataStream ile payload oluştur
              QByteArray payload;
           m_controlServer->sendToClient(
               address,
               port,
               ControlMessageType::StreamInfo,
               payload);

           break;
       }

       case ControlMessageType::StartStream:
       {
           // stream başlat

           m_controlServer->sendToClient(
               address,
               port,
               ControlMessageType::HelloAck);

           break;
       }


        case ControlMessageType::StartStreamFFmpegScreenCaptureAudio:
    {
        qInfo()
            << "Client requested START";

        // start streaming

        break;
    }

        case ControlMessageType::StartStreamFFmpegScreenCaptureAudioVideo:
    {
        qInfo()
            << "Client requested START";

        // start streaming

        break;
    }

        case ControlMessageType::StartStreamFFmpegScreenCaptureVideo:
        {
            qInfo()
                << "Client requested StartStreamFFmpegScreenCaptureVideo";

            m_controlServer->sendToClient(
                address,
                port,
                ControlMessageType::StartStream);
            //return;
            // start streaming
            StreamSettings settings;
            //settings.mode=StreamMode::Sender;
            settings.video.source = CaptureSource::ScreenFFmpeg;
            //settings.video.device="/dev/video2";
            settings.captureMode = CaptureMode::Video;
            //settings.previewMode = PreviewMode::Remote;
            //settings.videoAddress=QHostAddress("239.10.10.10");
            //settings.videoPort=5004;

            settings.audioSampleRate=48000;
            settings.audioChannels=2;
            settings.audioFrameSample=960;
            settings.audioBitrate=20000;
            settings.videoBitrate=1000;

            settings.audioAddress=settings.videoAddress;
            settings.audioPort=settings.videoPort+2;
            settings.video.screenIndex = 0;
            settings.video.fps    = 10;
            settings.video.width  = 160;
            settings.video.height = 90;
            settings.video.timeMode = TimeMode::MasterClock;

            /***********************************************************/
            const auto screens = QGuiApplication::screens();

            if (settings.video.screenIndex < 0 ||
                settings.video.screenIndex >= screens.size())
            {
                qCritical() << "Invalid screen index";
                return;
            }

            auto screen = screens.at(settings.video.screenIndex);

            //auto screen = QGuiApplication::screens().at(settings.video.screenIndex);

            int width  = screen->geometry().width();
            int height = screen->geometry().height();
            //qDebug() << "width:" << width;
            //qDebug() << "height:" << height;

            settings.video.width  = width;
            settings.video.height = height;
            if(settings.captureMode == CaptureMode::Video)
                settings.video.timeMode = TimeMode::FramePts;

            /***********************************************************/
            if (!this->open(settings))
            {
                qCritical() << "Failed to initialize MediaStreamer.";
                return;
            }
            if(!isStart)
            {
                this->start();
            }
            break;
        }


        case ControlMessageType::StopStream:
        {
            qInfo()
                << "Client requested STOP";

            // stop streaming

            break;
        }

        case ControlMessageType::Ping:
        {
            m_controlServer->sendToClient(
                address,
                port,
                ControlMessageType::Pong);

            break;
        }

         default:
            break;
    }
}
bool SenderPipeline::open( StreamSettings &settings)
{
    m_settings = settings;
    //return true;

    //
    if(settings.mode==StreamMode::Sender)
    {
        settings.video.timeMode = TimeMode::MasterClock;
    if (!prepareSource(settings))
        return false;

        /*qDebug()<<"xxxxxxxxxxxxxxxxxxxxxx";
        qDebug()<<"camera settings "<<
                  m_settings.video.width<<
                   m_settings.video.height<<
                  m_settings.video.fps;*/
    return true;
        //return m_sender.open(m_settings);
    }

     return true;
}

bool SenderPipeline::videoEnabled() const
{
    return m_settings.captureMode == CaptureMode::Video ||
           m_settings.captureMode == CaptureMode::AudioVideo;
}

bool SenderPipeline::audioEnabled() const
{
    return m_settings.captureMode== CaptureMode::Audio ||
           m_settings.captureMode == CaptureMode::AudioVideo;
}
bool SenderPipeline::localPreviewEnabled() const
{

}
bool SenderPipeline::remotePreviewEnabled() const
{

}

bool SenderPipeline::start()
{
    isStart=true;
    qDebug() << "RUNNING AS SENDER";

    setupClock();
    setupSession();

    switch (m_settings.video.source)
    {
    case CaptureSource::Camera:
        return startCamera();

    case CaptureSource::MediaFile:
        return startMediaFile();

    case CaptureSource::ScreenQt:
    case CaptureSource::ScreenFFmpeg:
    case CaptureSource::ScreenFFmpegPipeWire:
        return startScreen();

    default:
        return false;
    }
}

void SenderPipeline::setupClock()
{
    m_masterTime = new MasterTime(this);
    m_masterTime->start();
}

void SenderPipeline::setupSession()
{
    m_session = new SessionDescription(this);

    m_httpServer = new HttpServer(this);
    m_httpServer->setSession(m_session);
    m_httpServer->start(8080);

    m_rtspServer = new RtspServer(this);
    m_rtspServer->setSession(m_session);

    if (!m_rtspServer->start(8554))
        qDebug() << "RTSP server start failed";
    else
        qDebug() << "RTSP server started";

    connect(m_rtspServer,
            &RtspServer::playRequested,
            this,
            &SenderPipeline::rtspPlayRequested);

    connect(m_rtspServer,
            &RtspServer::teardownRequested,
            this,
            &SenderPipeline::rtspTeardownRequested);
}

bool SenderPipeline::startCamera()
{
    qDebug() << "START CAMERA";
    m_session->setVideoEnabled(false);
    m_session->setAudioEnabled(false);

    // AUDIO
   if (audioEnabled())
    {
        m_audioCapture = new AudioCapture;

        if (m_audioCapture->open(
                m_settings.audioSampleRate,
                m_settings.audioChannels))
        {

        m_audioCapture->setMasterTime(m_masterTime);

        m_audioSender = new AudioSender;
        m_audioSender->setMasterTime(m_masterTime);

        if (m_audioSender->open(
                m_settings.audioSampleRate,
                m_settings.audioChannels,
                m_settings.audioBitrate,
                QHostAddress(m_settings.audioAddress),
                m_settings.audioPort))
        {

        connect(m_audioCapture,
                &AudioCapture::frameReady,
                m_audioSender,
                &AudioSender::onFrame,
                Qt::QueuedConnection);
         m_session->setAudioEnabled(true);
        }
    }
}
    // VIDEO
    if (videoEnabled())
    {
        m_capture = new CaptureThread;

        if (!m_capture->open(m_settings.video))
            return false;

        m_capture->setMasterTime(m_masterTime);

        m_videoSender = new VideoSender;

        m_videoSender->setMasterTime(m_masterTime);

        if (m_videoSender->open(
                m_settings.video.width,
                m_settings.video.height,
                m_settings.video.fps,
                m_settings.videoBitrate,
                QHostAddress(m_settings.videoAddress),
                m_settings.videoPort))
        {

        connect(m_videoSender,
                &VideoSender::spsReady,
                m_session,
                &SessionDescription::setSps);

        connect(m_videoSender,
                &VideoSender::ppsReady,
                m_session,
                &SessionDescription::setPps);

        connect(m_capture,
                &CaptureThread::frameReady,
                m_videoSender,
                &VideoSender::onFrame,
                Qt::QueuedConnection);

        m_capture->start();
        m_session->setVideoEnabled(true);
        }
    }
    return true;
}

bool SenderPipeline::startMediaFile()
{
    qDebug() << "START MEDIA FILE";

    m_session->setVideoEnabled(false);
    m_session->setAudioEnabled(false);

    //
    // CAPTURE
    //
    if (videoEnabled() || audioEnabled())
    {
        m_capture = new CaptureThread;

        if (!m_capture->open(m_settings.video))
            return false;

        m_capture->setMasterTime(m_masterTime);
    }

    //
    // VIDEO
    //
    if (videoEnabled())
    {
        m_videoSender = new VideoSender;
        m_videoSender->setMasterTime(m_masterTime);

        if (m_videoSender->open(
                m_settings.video.width,
                m_settings.video.height,
                m_settings.video.fps,
                m_settings.videoBitrate,
                QHostAddress(m_settings.videoAddress),
                m_settings.videoPort))
        {


        connect(m_videoSender,
                &VideoSender::spsReady,
                m_session,
                &SessionDescription::setSps);

        connect(m_videoSender,
                &VideoSender::ppsReady,
                m_session,
                &SessionDescription::setPps);

        connect(m_capture,
                &CaptureThread::frameReady,
                m_videoSender,
                &VideoSender::onFrame,
                Qt::QueuedConnection);
         m_session->setVideoEnabled(true);
        }
    }

    //
    // AUDIO
    //
    if (audioEnabled())
    {
        m_audioSender = new AudioSender;
        m_audioSender->setMasterTime(m_masterTime);

        if (m_audioSender->open(
                m_settings.audioSampleRate,
                m_settings.audioChannels,
                m_settings.audioBitrate,
                QHostAddress(m_settings.audioAddress),
                m_settings.audioPort))
        {


        m_audioProcessor = new AudioProcessor(this);

        m_audioProcessor->setFormat(
            m_settings.audioSampleRate,
            m_settings.audioChannels);

        m_audioProcessor->setFrameSamples(
            m_settings.audioFrameSample);

        connect(m_capture,
                &CaptureThread::audioFrameReady,
                m_audioProcessor,
                &AudioProcessor::onFrame,
                Qt::QueuedConnection);

        connect(m_audioProcessor,
                &AudioProcessor::frameReady,
                m_audioSender,
                &AudioSender::onFrame,
                Qt::QueuedConnection);
         m_session->setAudioEnabled(true);
        }
    }

    //
    // START
    //
    if (m_capture)
        m_capture->start();

    return true;
}
bool SenderPipeline::startScreen()
{
    // start streaming
    StreamSettings settings;
    //settings.mode=StreamMode::Sender;
    settings.video.source = CaptureSource::ScreenFFmpeg;
    //settings.video.device="/dev/video2";
    //settings.captureMode = CaptureMode::Video;
    //settings.previewMode = PreviewMode::Remote;
    //settings.videoAddress=QHostAddress("239.10.10.10");
    //settings.videoPort=5004;

    settings.audioSampleRate=48000;
    settings.audioChannels=2;
    settings.audioFrameSample=960;
    settings.audioBitrate=20000;
    settings.videoBitrate=1000;

    settings.audioAddress=settings.videoAddress;
    settings.audioPort=settings.videoPort+2;
    settings.video.screenIndex = 0;
    settings.video.fps    = 10;
    settings.video.width  = 160;
    settings.video.height = 90;
    settings.video.timeMode = TimeMode::MasterClock;

    /***********************************************************/
    const auto screens = QGuiApplication::screens();

    if (settings.video.screenIndex < 0 ||
        settings.video.screenIndex >= screens.size())
    {
        qCritical() << "Invalid screen index";
        return false;
    }

    auto screen = screens.at(settings.video.screenIndex);

    //auto screen = QGuiApplication::screens().at(settings.video.screenIndex);

    int width  = screen->geometry().width();
    int height = screen->geometry().height();
    //qDebug() << "width:" << width;
    //qDebug() << "height:" << height;

    settings.video.width  = width;
    settings.video.height = height;
    if(settings.captureMode == CaptureMode::Video)
        settings.video.timeMode = TimeMode::FramePts;

    /***********************************************************/
    if (!this->open(settings))
    {
        qCritical() << "Failed to initialize MediaStreamer.";
        return false;
    }
   /* if(!isStart)
    {
        this->start();
    }*/
    qDebug() << "START SCREEN";

    m_session->setVideoEnabled(false);
    m_session->setAudioEnabled(false);

    //
    // VIDEO
    //
    if (videoEnabled())
    {
        m_capture = new CaptureThread;

        if (!m_capture->open(m_settings.video))
            return false;

        m_capture->setMasterTime(m_masterTime);

        m_videoSender = new VideoSender;
        m_videoSender->setMasterTime(m_masterTime);

        if (m_videoSender->open(
                m_settings.video.width,
                m_settings.video.height,
                m_settings.video.fps,
                m_settings.videoBitrate,
                QHostAddress(m_settings.videoAddress),
                m_settings.videoPort))
        {


        connect(m_videoSender,
                &VideoSender::spsReady,
                m_session,
                &SessionDescription::setSps);

        connect(m_videoSender,
                &VideoSender::ppsReady,
                m_session,
                &SessionDescription::setPps);

        connect(m_capture,
                &CaptureThread::frameReady,
                m_videoSender,
                &VideoSender::onFrame,
                Qt::QueuedConnection);
         m_session->setVideoEnabled(true);
        }
    }

    //
    // AUDIO
    //
    if (audioEnabled())
    {
        m_audioCapture = new AudioCapture;

        if (m_audioCapture->open(
                m_settings.audioSampleRate,
                m_settings.audioChannels))
        {
            m_audioCapture->setMasterTime(m_masterTime);

            m_audioSender = new AudioSender;
            m_audioSender->setMasterTime(m_masterTime);

            if (m_audioSender->open(
                    m_settings.audioSampleRate,
                    m_settings.audioChannels,
                    m_settings.audioBitrate,
                    QHostAddress(m_settings.audioAddress),
                    m_settings.audioPort))
            {


            connect(m_audioCapture,
                    &AudioCapture::frameReady,
                    m_audioSender,
                    &AudioSender::onFrame,
                    Qt::QueuedConnection);
            m_session->setAudioEnabled(true);
        }

        }
    }

    //
    // START CAPTURE
    //
    if (videoEnabled())
        m_capture->start();

    return true;
}



void SenderPipeline::stop()
{
    m_audioCapture->close();
    m_capture->stop();
    m_audioSender->close();
    m_videoSender->close();
    delete m_capture;
    m_capture = nullptr;
}

void SenderPipeline::rtspPlayRequested(const QHostAddress &addr,
                                       quint16 videoPort,
                                       quint16 audioPort)
{
    if (videoPort != 0)
    {
        if (!m_videoSender)
        {
            qWarning() << "RTSP PLAY: video requested but video sender is null";
            return;
        }

        m_videoSender->addClient(addr, videoPort);
    }

    if (audioPort != 0)
    {
        if (!m_audioSender)
        {
            qWarning() << "RTSP PLAY: audio requested but audio sender is null";
            return;
        }

        m_audioSender->addClient(addr, audioPort);
    }
}
/*
void SenderPipeline::rtspPlayRequested(const QHostAddress &addr,
                                      quint16 videoPort,
                                      quint16 audioPort)
{
    qDebug() << "PLAY REQUEST";
    qDebug() << addr;
    qDebug() << videoPort;
    qDebug() << audioPort;
    if (videoPort != 0)
        {
            m_videoSender->addClient(addr, videoPort);
        }

    if (audioPort != 0)
        {
            m_audioSender->addClient(addr, audioPort);
        }

    // Bir sonraki adım:
    // m_videoSender->addClient(addr, videoPort);
    //m_audioSender->addClient(addr, audioPort);
}
*/
void SenderPipeline::rtspTeardownRequested(
        const QHostAddress &addr,
        quint16 videoPort,
        quint16 audioPort)
{
    if (m_videoSender && videoPort != 0)
        m_videoSender->removeClient(addr, videoPort);

    if (m_audioSender && audioPort != 0)
        m_audioSender->removeClient(addr, audioPort);

    qDebug()
        << "CLIENT REMOVED"
        << addr
        << videoPort
        << audioPort;
}



bool SenderPipeline::prepareSource(StreamSettings &settings)
{

    if (settings.video.source == CaptureSource::MediaFile){

        MediaFileInfo info =
                MediaFileInfoReader::read(settings.video.fileName);

        if (!info.valid)
        {
            qDebug() << "Dosya açılamadı.";
            return false;
        }

       qDebug() << "Video :" << info.hasVideo;
        qDebug() << "Audio :" << info.hasAudio;
        qDebug() << "Size   :" << info.width << "x" << info.height;
        qDebug() << "FPS    :" << info.fps;
        qDebug() << "VCodec :" << info.videoCodec;
        qDebug() << "ACodec :" << info.audioCodec;
        qDebug() << "Rate   :" << info.sampleRate;
        qDebug() << "Channel:" << info.channels;

        settings.video.width  = info.width;
        settings.video.height = info.height;
        settings.video.timeMode = TimeMode::FramePts;
        settings.video.fps    = info.fps;
        //settings.audioSampleRate=info.sampleRate;
        //settings.audioFrameSample=882;
        settings.audioChannels=info.channels;
    }
    else if (settings.video.source == CaptureSource::Camera){

        CameraInfo info =
            CameraInfoReader::read(settings.video.device);

        if (!info.valid)
        {
            qDebug() << "Camera not found.";
            return false;
        }

       /* qDebug() << "Name   :" << info.name;
        qDebug() << "Driver :" << info.driver;
        qDebug() << "Bus    :" << info.bus;
        qDebug() << "Size   :" << info.width << "x" << info.height;
        qDebug() << "FPS    :" << info.fps;
        qDebug() << "Format :" << info.pixelFormat;*/

        ///settings.video.width  = info.width;
        ///settings.video.height = info.height;
        //settings.video.fps    = info.fps;

        if(settings.captureMode == CaptureMode::Video)
            settings.video.timeMode = TimeMode::FramePts;
    }
    else{
        const auto screens = QGuiApplication::screens();

        if (settings.video.screenIndex < 0 ||
            settings.video.screenIndex >= screens.size())
        {
            qCritical() << "Invalid screen index";
            return false;
        }

        auto screen = screens.at(settings.video.screenIndex);

        //auto screen = QGuiApplication::screens().at(settings.video.screenIndex);

        int width  = screen->geometry().width();
        int height = screen->geometry().height();
        //qDebug() << "width:" << width;
        //qDebug() << "height:" << height;

        settings.video.width  = width;
        settings.video.height = height;
        if(settings.captureMode == CaptureMode::Video)
            settings.video.timeMode = TimeMode::FramePts;
    }
    m_settings = settings;
    return true;
}


