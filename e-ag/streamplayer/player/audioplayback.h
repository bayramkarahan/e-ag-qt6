#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H

#include <QObject>
#include <QAudioSink>
#include <QAudioFormat>
#include <QIODevice>
#include<QMediaDevices>
#include<QAudioSink>
#include<QDateTime>
#include "../audio/audioframe.h"
#include "../rstp-sync/mediaclock.h"

class AudioPlayback : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayback(QObject *parent = nullptr);
    ~AudioPlayback();
    void setClock(MediaClock *clock);
    bool open(int sampleRate = 48000,
              int channels = 2);
    void close();

public slots:
    void onFrame(const AudioFrame &frame);

private:
    QAudioSink *m_sink = nullptr;
    QIODevice  *m_device = nullptr;

    QAudioFormat m_format;
    MediaClock *m_clock=nullptr;
};

#endif
