#ifndef AUDIOSCHEDULER_H
#define AUDIOSCHEDULER_H

#include <QObject>
#include <QQueue>
#include<QDebug>
#include<QTimer>
#include "../audio/audioframe.h"
#include "../sync/mediaclock.h"
#include "../common/VideoCaptureSettings.h"
class AudioScheduler : public QObject
{
    Q_OBJECT

public:
    explicit AudioScheduler(MediaClock *clock,
                            QObject *parent=nullptr);

public slots:
    void enqueue(const AudioFrame &frame);
    void open(const VideoCaptureSettings &settings);
    void clear();
signals:
    void frameReady(const AudioFrame &frame);
    void firstFrameReceived(qint64 pts);
private slots:

void processQueue();

private:
    MediaClock *m_clock;

    QQueue<AudioFrame> m_queue;
    bool m_started = false;
    TimeMode m_timeMode = TimeMode::MasterClock;

    QElapsedTimer m_localClock;
    qint64        m_firstPts = -1;

      qint64        m_firstPtsCount = -1;
    bool m_firstFrameReceived = false;
    QTimer m_timer;
};

#endif
