#include "videoscheduler.h"

#include <QElapsedTimer>
#include <QTimer>
#include <QDebug>

VideoScheduler::VideoScheduler(MediaClock *clock,
                               QObject *parent)
    : QObject(parent),
      m_clock(clock)
{
    connect(&m_timer,
            &QTimer::timeout,
            this,
            &VideoScheduler::processQueue);

    m_timer.start(1);


}

void VideoScheduler::processQueue()
{
    while (!m_queue.isEmpty())
    {
        const MediaFrame &frame = m_queue.head();

        qint64 clockUs;
        qint64 targetPts;

        if (m_timeMode == TimeMode::MasterClock)
        {
            if (!m_clock->isStarted())
            {
                qDebug() << "START MASTER CLOCK" << frame.pts;
                m_clock->start(frame.pts);
            }

            clockUs = m_clock->currentTimeUs();
            targetPts = frame.pts;
        }
        else
        {
            if (m_firstPts < 0)
            {
                m_firstPts = frame.pts;
                m_localClock.start();

                qDebug() << "START LOCAL CLOCK" << frame.pts;
            }

            clockUs = m_localClock.nsecsElapsed() / 1000;
            targetPts = frame.pts - m_firstPts;
        }

        qint64 diff = targetPts - clockUs;

        if (diff > 0)
            break;

        if (diff < -20000)
        {
            qDebug() << "DROP VIDEO" << diff;
            m_queue.dequeue();
            continue;
        }

        ///QThread::msleep(30);
        emit frameReady(frame);
        m_queue.dequeue();
    }
}
void VideoScheduler::enqueue(const MediaFrame &frame)
{
    static qint64 last = -1;

    if (last >= 0)
       /* qDebug() << "VIDEO"
                 << frame.pts
                 << "delta"
                 << frame.pts - last;*/

    last = frame.pts;
    m_queue.enqueue(frame);
}

void VideoScheduler::open(const VideoCaptureSettings &settings)
{
    m_timeMode = settings.timeMode;

    m_firstPts = -1;

    m_localClock.invalidate();

    while (!m_queue.isEmpty())
        m_queue.dequeue();
}


