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

    m_localClock.invalidate();

    while (!m_queue.isEmpty())
        m_queue.dequeue();
}

void AudioScheduler::processQueue()
{
    while (!m_queue.isEmpty())
    {
        const AudioFrame &frame = m_queue.head();
        // Clock daha başlamadıysa ilk frame ile başlat
            if (!m_clock->isStarted())
            {
                qDebug() << "START CLOCK" << frame.pts;
                m_clock->start(frame.pts);
            }
        qint64 now = m_clock->currentTimeUs();

        qint64 diff = frame.pts - now;

        if (diff > 0)
            break;

        if (diff < -20000)
        {
            qDebug() << "DROP AUDIO" << diff;
            m_queue.dequeue();
            continue;
        }
        ////m_clock->start(frame.pts);

        emit frameReady(frame);
        m_queue.dequeue();
        qDebug() << "AUDIO QUEUE LEFT" << m_queue.size();
    }
}
void AudioScheduler::enqueue(const AudioFrame &frame)
{
    static qint64 last = -1;

 qDebug() << "AUDIO QUEUE" << m_queue.size();

    last = frame.pts;

    m_queue.enqueue(frame);
}
