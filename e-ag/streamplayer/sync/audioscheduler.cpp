#include "audioscheduler.h"


AudioScheduler::AudioScheduler(MediaClock *clock,
                               QObject *parent)
    : QObject(parent),
      m_clock(clock)
{
    connect(&m_timer,
            &QTimer::timeout,
            this,
            &AudioScheduler::processQueue);

    m_timer.start(1);
}

void AudioScheduler::open(const VideoCaptureSettings &settings)
{
    m_timeMode = settings.timeMode;

    m_firstPts = -1;
    m_firstFrameReceived = false;
    m_localClock.invalidate();

    while (!m_queue.isEmpty())
        m_queue.dequeue();
}
void AudioScheduler::clear()
{
    while (!m_queue.isEmpty())
        m_queue.dequeue();

    m_firstPts = -1;
}

void AudioScheduler::processQueue()
{
    if (!m_clock->isStarted())
        return;

    qint64 now = m_clock->currentTimeUs();

    while (!m_queue.isEmpty())
    {
        const AudioFrame &frame = m_queue.head();

        qint64 diff = frame.pts - now;

        /*qDebug()
            << "AUDIO SCHEDULER CHECK"
            << "framePts =" << frame.pts
            << "clockUs =" << now
            << "diff =" << diff;*/

        if (diff > 0)
            break;

        if (diff < -50000)
        {
            m_queue.dequeue();
            continue;
        }

        emit frameReady(frame);
        m_queue.dequeue();

        now = m_clock->currentTimeUs();
    }
}

void AudioScheduler::enqueue(const AudioFrame &frame)
{
    if(m_firstPtsCount<3){
        m_firstPtsCount++;
        return;
    }

    if (!m_firstFrameReceived&&frame.pts>0)
    {
        m_firstFrameReceived = true;

        qDebug() << "AUDIO FIRST FRAME"
                 << frame.pts;

        emit firstFrameReceived(frame.pts);
    }

    m_queue.enqueue(frame);
}
