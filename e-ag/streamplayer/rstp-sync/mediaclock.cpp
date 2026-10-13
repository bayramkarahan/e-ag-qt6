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

void MediaClock::restart(qint64 pts)
{
    m_ptsBase = pts;
    m_timer.restart();
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

bool MediaClock::registerVideoFirstFrame(qint64 ptsUs)
{
    QMutexLocker locker(&m_mutex);

    if (m_videoFirstReceived)
        return false;

    m_firstVideoPts = ptsUs;
    m_videoFirstReceived = true;

    return true;
}

bool MediaClock::registerAudioFirstFrame(qint64 ptsUs)
{
    QMutexLocker locker(&m_mutex);

    if (m_audioFirstReceived)
        return false;

    m_firstAudioPts = ptsUs;
    m_audioFirstReceived = true;

    return true;
}

bool MediaClock::bothStreamsReady() const
{
    QMutexLocker locker(&m_mutex);

    return m_videoFirstReceived &&
           m_audioFirstReceived;
}

void MediaClock::startFromStreams()
{
    QMutexLocker locker(&m_mutex);

    if (m_started)
        return;

    if (!m_videoFirstReceived ||
        !m_audioFirstReceived)
        return;

    // Video ve audio başlangıçlarını ortak noktaya getir.
    m_ptsBase = qMin(m_firstVideoPts,
                     m_firstAudioPts);

    m_timer.start();
    m_started = true;

    qDebug() << "MASTER CLOCK START"
             << "videoPts =" << m_firstVideoPts
             << "audioPts =" << m_firstAudioPts
             << "base =" << m_ptsBase;
}
