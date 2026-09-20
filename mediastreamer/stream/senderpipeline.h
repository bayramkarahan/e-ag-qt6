#pragma once

#include <QObject>

#include "thread/capturethread.h"

#include "audio/audiocapture.h"
#include "player/audioprocessor.h"

#include "sender/videosender.h"
#include "sender/audiosender.h"

#include "receiver/rtpreceiver.h"

#include "decoder/h264depacketizer.h"
#include "decoder/h264decoder.h"

#include "decoder/audioopusdecoder.h"

#include "player/audioplayback.h"

#include "renderer/videorenderer.h"
#include "preview/videopreview.h"

#include "sync/mastertime.h"
#include "sync/mediaclock.h"
#include "sync/videoscheduler.h"
#include "sync/audioscheduler.h"
#include "session/httpserver.h"
#include "session/rtspserver.h"
#include "session/sessiondescription.h"
#include "../common/StreamSettings.h"

#include "common/mediafileinforeader.h"
#include "common/camerainforeader.h"
#include "../udpmesageservice/controlserver.h"

#include<QGuiApplication>

class SenderPipeline : public QObject
{
    Q_OBJECT

public:
    explicit SenderPipeline(QObject *parent = nullptr);

    bool open(StreamSettings &lsettings);
    bool start();
    void stop();
    void rtspPlayRequested(const QHostAddress &addr,
                                          quint16 videoPort,
                                          quint16 audioPort);
    void rtspTeardownRequested(
            const QHostAddress &addr,
            quint16 videoPort,
            quint16 audioPort);
    bool prepareSource(StreamSettings &settings);

private:

    bool openDevices();
    void connectObjects();
    bool audioEnabled() const;
    bool videoEnabled() const;
    bool localPreviewEnabled() const;
    bool remotePreviewEnabled() const;
    bool startCamera();
    bool startMediaFile();
    bool startScreen();
    void setupClock();
    void setupSession();
    void onControlMessage(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);
private:

    StreamSettings m_settings;

    MasterTime* m_masterTime = nullptr;
    MediaClock* m_clock = nullptr;

    CaptureThread* m_capture = nullptr;
    AudioCapture* m_audioCapture = nullptr;
    AudioProcessor* m_audioProcessor = nullptr;

    VideoSender* m_videoSender = nullptr;
    AudioSender* m_audioSender = nullptr;

    SessionDescription* m_session = nullptr;

    // Local preview

    RTPReceiver* m_videoReceiver = nullptr;
    RTPReceiver* m_audioReceiver = nullptr;

    H264Depacketizer* m_depacketizer = nullptr;
    H264Decoder* m_videoDecoder = nullptr;

    AudioOpusDecoder* m_audioDecoder = nullptr;

    AudioPlayback* m_playback = nullptr;

    VideoScheduler* m_videoScheduler = nullptr;
    AudioScheduler* m_audioScheduler = nullptr;

    VideoRenderer* m_renderer = nullptr;
    VideoPreview* m_preview = nullptr;
    HttpServer *m_httpServer;
    RtspServer *m_rtspServer;
    bool isStart=false;
    ControlServer *m_controlServer = nullptr;
};

