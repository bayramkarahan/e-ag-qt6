#ifndef VIDEOSCHEDULER_H
#define VIDEOSCHEDULER_H

#include <QObject>
#include <QQueue>
#include<QTimer>
#include "common/MediaFrame.h"
#include "sync/mediaclock.h"
#include "common/VideoCaptureSettings.h"
#include<QThread>
class VideoScheduler : public QObject
{
    Q_OBJECT

public:
    explicit VideoScheduler(MediaClock *clock,
                            QObject *parent=nullptr);

public slots:
    void enqueue(const MediaFrame &frame);
    void open(const VideoCaptureSettings &settings);

private slots:

void processQueue();

signals:
    void frameReady(const MediaFrame &frame);

private:
    MediaClock *m_clock;

    QQueue<MediaFrame> m_queue;
    TimeMode m_timeMode = TimeMode::MasterClock;

    QElapsedTimer m_localClock;
    qint64        m_firstPts = -1;


    QTimer m_timer;
};

#endif
