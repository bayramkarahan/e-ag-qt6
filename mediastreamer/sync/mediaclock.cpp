#include "mediaclock.h"

MediaClock::MediaClock(QObject *parent)
    : QObject(parent)
{
}
void MediaClock::start(qint64 ptsUs)
{
    QMutexLocker locker(&m_mutex);

    if (m_started)
        return;

    m_ptsBase = ptsUs;
    m_timer.start();
    m_started = true;
}

bool MediaClock::isStarted() const
{
    QMutexLocker locker(&m_mutex);
    return m_started;
}
void MediaClock::reset(qint64 ptsUs)
{
    QMutexLocker locker(&m_mutex);

    m_ptsBase = ptsUs;

    m_timer.restart();

    m_started = true;
}

qint64 MediaClock::currentTimeUs() const
{
    QMutexLocker locker(&m_mutex);

    if(!m_started)
        return 0;

    return m_ptsBase +
           (m_timer.nsecsElapsed() / 1000);
}
void MediaClock::adjust(qint64 ptsUs)
{
    QMutexLocker locker(&m_mutex);

    if (!m_started)
        return;

    qint64 elapsed = m_timer.nsecsElapsed() / 1000;

    // Şu anki zamanı verilen PTS yap
    ///m_ptsBase = ptsUs - elapsed;
    qint64 desiredBase = ptsUs - elapsed;

    m_ptsBase += (desiredBase - m_ptsBase) / 8;
}
