#pragma once

#include <QObject>
#include <QThread>
#include <QAtomicInteger>

#include "capture/mediafilecapture.h"

class MediaAudioThread : public QObject
{
    Q_OBJECT

public:
    explicit MediaAudioThread(MediaFileCapture *capture,
                              QObject *parent = nullptr);

    void start();
    void stop();

signals:
    void frameReady(const AudioFrame &frame);

private slots:
    void process();

private:
    MediaFileCapture *m_capture = nullptr;

    QThread m_thread;
    QAtomicInteger<int> m_running = 0;
    VideoCaptureSettings m_settings;
};
