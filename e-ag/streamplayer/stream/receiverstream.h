#pragma once

#include <QObject>
#include <QHostAddress>

#include "../common/VideoCaptureSettings.h"
#include "../common/StreamSettings.h"
#include "../player/rtspsource.h"
#include "../player/rtspworker.h"
#include "../udpmesageservice/controlclient.h"
class RTPReceiver;
class H264Depacketizer;
class H264Decoder;
class AudioOpusDecoder;
class AudioPlayback;
class VideoRenderer;
class VideoPreview;
class OpenGLVideoRenderer;
class OpenGLVaapiZeroCopyRenderer;
class OpenGLVideoPreview;
class MasterTime;
class MediaClock;
class VideoScheduler;
class AudioScheduler;
class AbstractVideoDecoder;
class CpuVideoDecoder;
class AbstractVideoRenderer;
class ReceiverStream : public QObject
{
    Q_OBJECT

public:
    explicit ReceiverStream(QObject *parent = nullptr);
 ~ReceiverStream();
    bool open(const StreamSettings &settings);
    bool start(QString hostIp);
    void stop();
    void tryStartPlayback();
    AbstractVideoRenderer*     m_openglrenderer = nullptr;
    void onServerFound(
        const QHostAddress &address,
        quint16 port);
    void onControlMessage(
        const ControlMessage &message);
private:

    StreamSettings m_settings;

    MasterTime*        m_masterTime = nullptr;
    MediaClock*        m_clock = nullptr;

    VideoRenderer*     m_renderer = nullptr;
    VideoPreview*      m_preview = nullptr;
    //OpenGLVaapiZeroCopyRenderer*     m_openglvaapizerorenderer = nullptr;

    OpenGLVideoPreview*      m_openglpreview = nullptr;

    RTPReceiver*       m_videoReceiver = nullptr;
    RTPReceiver*       m_audioReceiver = nullptr;

    H264Depacketizer*  m_depacketizer = nullptr;
    //H264Decoder*       m_videoDecoder = nullptr;
    AbstractVideoDecoder *m_videoDecoder = nullptr;
    //CpuVideoDecoder *m_videoDecoder = nullptr;

    AudioOpusDecoder*  m_audioDecoder = nullptr;

    VideoScheduler*    m_videoScheduler = nullptr;
    AudioScheduler*    m_audioScheduler = nullptr;

    AudioPlayback*     m_playback = nullptr;
    //RTPJitterBuffer *m_videoJitter=nullptr;
    //RTPJitterBuffer *m_audioJitter=nullptr;
    RtspSource   *m_rtspSource=nullptr;
    bool m_decoderStarted = false;

    bool m_playbackSynchronized = false;
    bool m_videoStarted = false;
    bool m_audioStarted = false;
    bool m_playbackStarted = false;

    qint64 m_firstVideoPts = -1;
    qint64 m_firstAudioPts = -1;

    ControlClient *m_controlClient = nullptr;
};
