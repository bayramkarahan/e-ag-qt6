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
    qDebug() << "VideoScheduler THREAD ="
             << QThread::currentThread();

}

void VideoScheduler::clear()
{
    while (!m_queue.isEmpty())
        m_queue.dequeue();

    m_firstPts = -1;
    m_localClock.invalidate();
}

void VideoScheduler::processQueue()
{
    if (m_timeMode == TimeMode::MasterClock &&
        !m_clock->isStarted())
        return;

    while (!m_queue.isEmpty())
    {
        const MediaFrame &frame = m_queue.head();

        qint64 clockUs;
        qint64 targetPts;

        if (m_timeMode == TimeMode::MasterClock)
        {
            clockUs = m_clock->currentTimeUs();
            targetPts = frame.pts;
        }
        else
        {
            if (m_firstPts < 0)
            {
                m_firstPts = frame.pts;
                m_localClock.start();

                qDebug() << "START LOCAL CLOCK"
                         << frame.pts;
            }

            clockUs = m_localClock.nsecsElapsed() / 1000;
            targetPts = frame.pts - m_firstPts;
        }

        qint64 diff = targetPts - clockUs;

        /*qDebug()
            << "VIDEO SCHEDULER CHECK"
            << "framePts =" << frame.pts
            << "clockUs =" << clockUs
            << "diff =" << diff;*/

        if (diff > 0)
            break;

        if (diff < -20000)
        {
            qDebug() << "DROP VIDEO"
                     << "pts =" << frame.pts
                     << "clock =" << clockUs
                     << "diff =" << diff;

            m_queue.dequeue();
            continue;
        }

        emit frameReady(frame);
        m_queue.dequeue();
    }
}

void VideoScheduler::enqueue(const MediaFrame &frame)
{
    if (!m_firstFrameReceived)
    {
        m_firstFrameReceived = true;

        qDebug() << "VIDEO FIRST FRAME"
                 << frame.pts;

        emit firstFrameReceived(frame.pts);
    }

    m_queue.enqueue(frame);
}
void VideoScheduler::open(const VideoCaptureSettings &settings)
{
    m_timeMode = settings.timeMode;
    m_firstFrameReceived = false;
    m_firstPts = -1;

    m_localClock.invalidate();

    while (!m_queue.isEmpty())
        m_queue.dequeue();
}


