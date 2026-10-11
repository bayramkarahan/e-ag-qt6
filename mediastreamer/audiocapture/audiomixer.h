
#ifndef AUDIOMIXER_H
#define AUDIOMIXER_H

#include <QObject>
#include <QQueue>

#include "audioframe.h"

class AudioMixer : public QObject
{
    Q_OBJECT

public:
    explicit AudioMixer(QObject *parent = nullptr);

public slots:
    void onMicrophoneFrame(const AudioFrame &frame);
    void onSystemFrame(const AudioFrame &frame);

signals:
    void frameReady(const AudioFrame &frame);

private:
    void mixAvailableFrames();
    AudioFrame mixFrames(const AudioFrame &mic,
                         const AudioFrame &system);

    QQueue<AudioFrame> m_microphoneQueue;
    QQueue<AudioFrame> m_systemQueue;

    static constexpr int MaxQueueSize = 5;
};

#endif
