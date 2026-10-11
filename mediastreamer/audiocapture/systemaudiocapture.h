
#ifndef SYSTEMAUDIOCAPTURE_H
#define SYSTEMAUDIOCAPTURE_H

#include <QObject>
#include <QByteArray>
#include <QMutex>

#include <pulse/pulseaudio.h>
#include <pulse/simple.h>
#include <atomic>
#include <thread>
#include "sync/mastertime.h"
#include "audioframe.h"

class SystemAudioCapture : public QObject
{
    Q_OBJECT

public:
    explicit SystemAudioCapture(QObject *parent = nullptr);
    ~SystemAudioCapture();

    void setMasterTime(MasterTime *masterTime);

    bool open(int sampleRate = 48000,
              int channels = 2);

    void close();

signals:
    void frameReady(const AudioFrame &frame);

private:
    void captureLoop();

    pa_simple *m_stream = nullptr;

    QByteArray m_buffer;
    QMutex m_mutex;

    MasterTime *m_masterTime = nullptr;

    int m_sampleRate = 48000;
    int m_channels = 2;
    std::atomic_bool m_running{false};
    std::thread m_thread;

    qint64 m_pts = 0;
};

#endif
