#pragma once

#include <QObject>

#include "thread/capturethread.h"
#include "../audiocapture/microphoneaudiocapture.h"
#include "../audiocapture/systemaudiocapture.h"
#include "player/audioprocessor.h"
#include "sender/videosender.h"
#include "sender/audiosender.h"

#include "../audiocapture/audiomixer.h"
#include "sync/mastertime.h"
#include "session/httpserver.h"
#include "session/rtspserver.h"
#include "session/sessiondescription.h"
#include "../common/StreamSettings.h"
#include <QScreen>
#include "common/mediafileinforeader.h"
#include "common/camerainforeader.h"
#include "../udpmesageservice/controlserver.h"
#include "../videocapture/desktopsession.h"
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
    bool startX11ScreenCapture();
    bool startWaylandScreenCapture();
    void setupClock();
    void setupSession();
    void onControlMessage(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);
private:

    StreamSettings m_settings;

    MasterTime* m_masterTime = nullptr;

    CaptureThread* m_capture = nullptr;
    MicrofonAudioCapture* m_microphoneAudioCapture = nullptr;
    SystemAudioCapture* m_systemAudioCapture = nullptr;
    AudioProcessor* m_audioProcessor = nullptr;
    AudioMixer *m_audioMixer = nullptr;
    VideoSender* m_videoSender = nullptr;
    AudioSender* m_audioSender = nullptr;

    SessionDescription* m_session = nullptr;

    // Local preview

    HttpServer *m_httpServer;
    RtspServer *m_rtspServer;
    bool isStart=false;
    ControlServer *m_controlServer = nullptr;
};

