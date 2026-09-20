#pragma once

#include <QObject>
#include <QHostAddress>

#include "../common/VideoCaptureSettings.h"
#include "../common/StreamSettings.h"
#include "session/sessiondescription.h"
#include "../network/rtpjitterbuffer.h"
class RTPReceiver;
class H264Depacketizer;
class H264Decoder;
class AudioOpusDecoder;
class AudioPlayback;
class VideoRenderer;
class VideoPreview;
class MasterTime;
class MediaClock;
class VideoScheduler;
class AudioScheduler;
class ReceiverPipeline : public QObject
{
    Q_OBJECT

public:
    explicit ReceiverPipeline(QObject *parent = nullptr);

    bool open(const StreamSettings &settings);
    bool start();
    void stop();
private:

    bool createObjects();
    bool openDevices();
    void connectObjects();

private:

    StreamSettings m_settings;

    MasterTime*        m_masterTime = nullptr;
    MediaClock*        m_clock = nullptr;

    VideoRenderer*     m_renderer = nullptr;
    VideoPreview*      m_preview = nullptr;

    RTPReceiver*       m_videoReceiver = nullptr;
    RTPReceiver*       m_audioReceiver = nullptr;

    H264Depacketizer*  m_depacketizer = nullptr;
    H264Decoder*       m_videoDecoder = nullptr;

    AudioOpusDecoder*  m_audioDecoder = nullptr;

    VideoScheduler*    m_videoScheduler = nullptr;
    AudioScheduler*    m_audioScheduler = nullptr;

    AudioPlayback*     m_playback = nullptr;
    RTPJitterBuffer *m_videoJitter=nullptr;
    RTPJitterBuffer *m_audioJitter=nullptr;
};
