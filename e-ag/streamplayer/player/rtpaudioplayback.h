#ifndef RTPAUDIOPLAYBACK_H
#define RTPAUDIOPLAYBACK_H

#include <QObject>
#include <QAudioSink>
#include <QAudioFormat>
#include <QIODevice>
#include<QMediaDevices>
#include<QAudioSink>
#include<QDateTime>
#include "../audio/audioframe.h"
#include "../rtp-sync/rtpmediaclock.h"

class RtpAudioPlayback : public QObject
{
    Q_OBJECT

public:
    explicit RtpAudioPlayback(QObject *parent = nullptr);
    ~RtpAudioPlayback();
    void setClock(RtpMediaClock *clock);
    bool open(int sampleRate = 48000,
              int channels = 2);

    void close();

public slots:
    void onFrame(const AudioFrame &frame);

private:

    QAudioSink *m_sink = nullptr;
    QIODevice  *m_device = nullptr;

    QAudioFormat m_format;
    RtpMediaClock *m_clock=nullptr;
};

#endif
