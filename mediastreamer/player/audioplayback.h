#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H

#include <QObject>
#include <QAudioSink>
#include <QAudioFormat>
#include <QIODevice>
#include<QMediaDevices>
#include<QAudioSink>
#include<QDateTime>
#include "sync/mastertime.h"
#include "audio/audioframe.h"
#include "sync/mediaclock.h"

class AudioPlayback : public QObject
{
    Q_OBJECT

public:
    explicit AudioPlayback(QObject *parent = nullptr);
    ~AudioPlayback();
    void setClock(MediaClock *clock);
    bool open(int sampleRate = 48000,
              int channels = 2);
    void setMasterTime(MasterTime *masterTime);
    void close();

public slots:
    void onFrame(const AudioFrame &frame);

private:
    MasterTime *m_masterTime = nullptr;
    QAudioSink *m_sink = nullptr;
    QIODevice  *m_device = nullptr;

    QAudioFormat m_format;
    MediaClock *m_clock=nullptr;
};

#endif
