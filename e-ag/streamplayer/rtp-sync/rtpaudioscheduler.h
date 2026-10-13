#ifndef RTPAUDIOSCHEDULER_H
#define RTPAUDIOSCHEDULER_H

#include <QObject>
#include <QQueue>
#include<QDebug>
#include<QTimer>
#include "../audio/audioframe.h"
#include "../rtp-sync/rtpmediaclock.h"
#include "../common/StreamSettings.h"

class RtpAudioScheduler : public QObject
{
    Q_OBJECT

public:
    explicit RtpAudioScheduler(RtpMediaClock *clock,
                            QObject *parent=nullptr);

public slots:
    void enqueue(const AudioFrame &frame);
    void open(const VideoCaptureSettings &settings);

signals:
    void frameReady(const AudioFrame &frame);
private slots:

void processQueue();

private:
    RtpMediaClock *m_clock;

    QQueue<AudioFrame> m_queue;
    bool m_started = false;
    TimeMode m_timeMode = TimeMode::MasterClock;

    QElapsedTimer m_localClock;
    qint64        m_firstPts = -1;



    QTimer m_timer;
};

#endif
