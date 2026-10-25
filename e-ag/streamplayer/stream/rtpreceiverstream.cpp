#include "rtpreceiverstream.h"
#include "../network/rtpjitterbuffer.h"
#include "../decoder/audioopusdecoder.h"
#include "../decoder/h264depacketizer.h"
#include "../decoder/h264decoder.h"
#include "../decoder/CpuVideoDecoder.h"
#include "../decoder/AbstractVideoDecoder.h"
#include "../decoder/videodecoderfactory.h"
#include "../player/rtpaudioplayback.h"
#include "../preview/openglvaapizerocopypreview.h"
#include "../preview/openglvideopreview.h"
#include "../preview/videopreview.h"
#include "../preview/AbstractVideoPreview.h"
#include "../rtp-sync/rtpmediaclock.h"
#include "../rtp-sync/rtpvideoscheduler.h"
#include "../rtp-sync/rtpaudioscheduler.h"

RtpReceiverStream::RtpReceiverStream(QObject *parent)
    : QObject(parent)
{

}


bool RtpReceiverStream::open(const StreamSettings &settings)
{
    m_settings = settings;
    return true;
}

bool RtpReceiverStream::startAudio()
{
    qDebug() << "RUNNING AS RTP AUDIO RECEIVER";

    m_audioScheduler = new RtpAudioScheduler(m_clock, this);
    m_audioScheduler->open(m_settings.video);


    m_audioDecoder = new AudioOpusDecoder(this);

    if (!m_audioDecoder->open(
            m_settings.audio.sampleRate,
            m_settings.audio.channels))
    {
        qCritical() << "AudioOpusDecoder açılamadı";
        return false;
    }

    m_playback = new RtpAudioPlayback(this);

    if (!m_playback->open(
            m_settings.audio.sampleRate,
            m_settings.audio.channels))
    {
        qCritical() << "AudioPlayback açılamadı";
        return false;
    }

    m_playback->setClock(m_clock);

    m_audioJitterBuffer = new RTPJitterBuffer(this);

    connect(m_rtpSource,
            &RtpSource::audioPacket,
            m_audioJitterBuffer,
            &RTPJitterBuffer::processPacket,
            Qt::QueuedConnection);

    connect(m_audioJitterBuffer,
            &RTPJitterBuffer::packetReady,
            m_audioDecoder,
            &AudioOpusDecoder::decodeRtp,
            Qt::QueuedConnection);

    connect(m_audioDecoder,
            &AudioOpusDecoder::frameReady,
            m_audioScheduler,
            &RtpAudioScheduler::enqueue,
            Qt::QueuedConnection);

    connect(m_audioScheduler,
            &RtpAudioScheduler::frameReady,
            m_playback,
            &RtpAudioPlayback::onFrame,
            Qt::QueuedConnection);

    if (!m_rtpSource->open(m_settings))
        return false;

    m_rtpSource->start();

    return true;
}

bool RtpReceiverStream::startVideo()
{
    qDebug() << "RUNNING AS RTP VIDEO RECEIVER";

    m_videoScheduler = new RtpVideoScheduler(m_clock, this);
    m_videoScheduler->open(m_settings.video);

    // --------------------------------------------------
    // VIDEO DECODER
    // --------------------------------------------------

    m_videoDecoder =VideoDecoderFactory::create(this, m_settings);

    if (!m_videoDecoder)
    {
        qCritical() << "No usable video decoder";
        return false;
    }
    m_videoDecoder->setSettings(m_settings);

    // --------------------------------------------------
    // RENDERER
    // --------------------------------------------------
/*
    if (m_videoDecoder->type() ==
        AbstractVideoDecoder::Type::VAAPI)
    {
        qDebug() << "Renderer: VAAPI";
        m_openglrenderer = new OpenGLVaapiZeroCopyRenderer;
    }
    else
    {
        qDebug() << "Renderer: CPU";
        m_openglrenderer = new OpenGLVideoRenderer;
    }*/
    m_preview = new VideoPreview;
    m_preview->setSettings(m_settings);
    // --------------------------------------------------
    // PREVIEW
    // --------------------------------------------------

    if (m_settings.previewMode == PreviewMode::NormalPreview)
    {
        m_preview->resize(
            m_settings.video.width,
            m_settings.video.height);

        m_preview->show();
    }
    else if (m_settings.previewMode == PreviewMode::FullScreenPreview)
    {
        const QList<QScreen *> screens = QGuiApplication::screens();

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

    // --------------------------------------------------
    // RTP VIDEO PIPELINE
    // --------------------------------------------------

    m_videoJitterBuffer = new RTPJitterBuffer(this);
    m_h264Depacketizer = new H264Depacketizer(this);

    connect(m_rtpSource,
            &RtpSource::videoPacket,
            m_videoJitterBuffer,
            &RTPJitterBuffer::processPacket,
            Qt::QueuedConnection);

    connect(m_videoJitterBuffer,
            &RTPJitterBuffer::packetReady,
            m_h264Depacketizer,
            &H264Depacketizer::processPacket,
            Qt::QueuedConnection);

    connect(m_h264Depacketizer,
            &H264Depacketizer::nalReady,
            m_videoDecoder,
            &AbstractVideoDecoder::decode,
            Qt::DirectConnection);

    connect(m_videoDecoder,
            &AbstractVideoDecoder::frameReady,
            m_videoScheduler,
            &RtpVideoScheduler::enqueue,
            Qt::QueuedConnection);

    connect(m_videoScheduler,
            &RtpVideoScheduler::frameReady,
            m_preview,
            &VideoPreview::onFrame,
            Qt::QueuedConnection);

    // --------------------------------------------------
    // START RTP SOURCE
    // --------------------------------------------------

    if (!m_rtpSource->open(m_settings))
        return false;

    m_rtpSource->start();

    return true;
}

bool RtpReceiverStream::startAudioVideo()
{
    qDebug() << "RUNNING AS RTP AUDIO/VIDEO RECEIVER";

    // --------------------------------------------------
    // SCHEDULERS
    // --------------------------------------------------

    m_audioScheduler = new RtpAudioScheduler(m_clock, this);
    m_audioScheduler->open(m_settings.video);

    m_videoScheduler = new RtpVideoScheduler(m_clock, this);
    m_videoScheduler->open(m_settings.video);

    // --------------------------------------------------
    // VIDEO DECODER
    // --------------------------------------------------

    m_videoDecoder =VideoDecoderFactory::create(this, m_settings);

    if (!m_videoDecoder)
    {
        qCritical() << "No usable video decoder";
        return false;
    }
    m_videoDecoder->setSettings(m_settings);

    // --------------------------------------------------
    // VIDEO RENDERER
    // --------------------------------------------------

    /*if (m_videoDecoder->type() ==
        AbstractVideoDecoder::Type::VAAPI)
    {
        qDebug() << "Renderer: VAAPI";
        m_openglrenderer = new OpenGLVaapiZeroCopyRenderer;
    }
    else
    {
        qDebug() << "Renderer: CPU";
        m_openglrenderer = new OpenGLVideoRenderer;
    }*/
    m_preview = new VideoPreview;
    m_preview->setSettings(m_settings);

    // --------------------------------------------------
    // VIDEO PREVIEW
    // --------------------------------------------------

    if (m_settings.previewMode == PreviewMode::NormalPreview)
    {
        m_preview->resize(
            m_settings.video.width,
            m_settings.video.height);

        m_preview->show();
    }
    else if (m_settings.previewMode == PreviewMode::FullScreenPreview)
    {
        const QList<QScreen *> screens = QGuiApplication::screens();

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

        m_preview->setWindowFlags(
            Qt::FramelessWindowHint |
            Qt::WindowStaysOnTopHint |
            Qt::X11BypassWindowManagerHint);

        m_preview->setGeometry(geometry);
        m_preview->show();
        m_preview->raise();
    }

    // --------------------------------------------------
    // VIDEO PIPELINE
    // --------------------------------------------------

    m_videoJitterBuffer = new RTPJitterBuffer(this);
    m_h264Depacketizer = new H264Depacketizer(this);

    connect(m_rtpSource,
            &RtpSource::videoPacket,
            m_videoJitterBuffer,
            &RTPJitterBuffer::processPacket,
            Qt::QueuedConnection);

    connect(m_videoJitterBuffer,
            &RTPJitterBuffer::packetReady,
            m_h264Depacketizer,
            &H264Depacketizer::processPacket,
            Qt::QueuedConnection);

    connect(m_h264Depacketizer,
            &H264Depacketizer::nalReady,
            m_videoDecoder,
            &AbstractVideoDecoder::decode,
            Qt::DirectConnection);

    connect(m_videoDecoder,
            &AbstractVideoDecoder::frameReady,
            m_videoScheduler,
            &RtpVideoScheduler::enqueue,
            Qt::QueuedConnection);

    connect(m_videoScheduler,
            &RtpVideoScheduler::frameReady,
            m_preview,
            &VideoPreview::onFrame,
            Qt::QueuedConnection);

    // --------------------------------------------------
    // AUDIO DECODER
    // --------------------------------------------------

    m_audioDecoder = new AudioOpusDecoder;

    if (!m_audioDecoder->open(
            m_settings.audio.sampleRate,
            m_settings.audio.channels))
    {
        qCritical() << "AudioOpusDecoder açılamadı";
        return false;
    }

    // --------------------------------------------------
    // AUDIO PLAYBACK
    // --------------------------------------------------

    m_playback = new RtpAudioPlayback;

    if (!m_playback->open(
            m_settings.audio.sampleRate,
            m_settings.audio.channels))
    {
        qCritical() << "AudioPlayback açılamadı";
        return false;
    }

    m_playback->setClock(m_clock);

    // --------------------------------------------------
    // AUDIO PIPELINE
    // --------------------------------------------------

    m_audioJitterBuffer = new RTPJitterBuffer(this);

    connect(m_rtpSource,
            &RtpSource::audioPacket,
            m_audioJitterBuffer,
            &RTPJitterBuffer::processPacket,
            Qt::QueuedConnection);

    connect(m_audioJitterBuffer,
            &RTPJitterBuffer::packetReady,
            m_audioDecoder,
            &AudioOpusDecoder::decodeRtp,
            Qt::QueuedConnection);

    connect(m_audioDecoder,
            &AudioOpusDecoder::frameReady,
            m_audioScheduler,
            &RtpAudioScheduler::enqueue,
            Qt::QueuedConnection);

    connect(m_audioScheduler,
            &RtpAudioScheduler::frameReady,
            m_playback,
            &RtpAudioPlayback::onFrame,
            Qt::QueuedConnection);

    // --------------------------------------------------
    // START RTP SOURCE ONCE
    // --------------------------------------------------

    if (!m_rtpSource->open(m_settings))
        return false;

    m_rtpSource->start();

    return true;
}


bool RtpReceiverStream::start()
{
    qDebug() << "RUNNING AS RTP MULTICAST RECEIVER";

    // Ortak nesneler
    m_clock = new RtpMediaClock(this);
    m_rtpSource = new RtpSource(this);

    connect(m_rtpSource, &RtpSource::connected,
            this, []() {
        qDebug() << "RTP Multicast Connected";
    });

    connect(m_rtpSource, &RtpSource::disconnected,
            this, []() {
        qDebug() << "RTP Multicast Disconnected";
    });

    connect(m_rtpSource, &RtpSource::errorOccurred,
            this, [](const QString &err) {
        qCritical() << "RTP ERROR:" << err;
    });
    if(m_settings.audio.source!=AudioSource::None&&
            m_settings.video.source==VideoSource::None)
        return startAudio();

    if(m_settings.audio.source==AudioSource::None&&
            m_settings.video.source!=VideoSource::None)
        return startVideo();

    if(m_settings.audio.source!=AudioSource::None&&
            m_settings.video.source!=VideoSource::None)
        return startAudioVideo();

    return false;
}

void RtpReceiverStream::tryStartPlayback()
{
    if (m_clock->isStarted())
        return;

    if (m_audioStarted)
    {
        qDebug() << "START MASTER CLOCK ="
                 << m_firstAudioPts;

        m_clock->start(m_firstAudioPts);
    }
    else if (m_videoStarted)
    {
        qDebug() << "START MASTER CLOCK ="
                 << m_firstVideoPts;

        m_clock->start(m_firstVideoPts);
    }
}
