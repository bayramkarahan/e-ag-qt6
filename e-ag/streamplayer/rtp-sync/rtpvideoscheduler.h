#ifndef RTPVIDEOSCHEDULER_H
#define RTPVIDEOSCHEDULER_H

#include <QObject>
#include <QQueue>
#include<QTimer>
#include "../common/MediaFrame.h"
#include "../rtp-sync/rtpmediaclock.h"
#include "../common/StreamSettings.h"
#include<QThread>
class RtpVideoScheduler : public QObject
{
    Q_OBJECT

public:
    explicit RtpVideoScheduler(RtpMediaClock *clock,
                            QObject *parent=nullptr);

public slots:
    void enqueue(const MediaFrame &frame);
    void open(const VideoCaptureSettings &settings);

private slots:

void processQueue();

signals:
    void frameReady(const MediaFrame &frame);

private:
    RtpMediaClock *m_clock;

    QQueue<MediaFrame> m_queue;
    TimeMode m_timeMode = TimeMode::MasterClock;

    QElapsedTimer m_localClock;
    qint64        m_firstPts = -1;


    QTimer m_timer;
};

#endif
