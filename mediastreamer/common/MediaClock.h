#ifndef MEDIACLOCK_H
#define MEDIACLOCK_H

#include <QElapsedTimer>

class MediaClock
{
public:

    MediaClock()
    {
        m_timer.start();
    }

    qint64 us() const
    {
        return m_timer.nsecsElapsed()/1000;
    }

private:

    QElapsedTimer m_timer;
};

#endif
