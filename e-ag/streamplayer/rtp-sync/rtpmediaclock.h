#ifndef RTPMEDIACLOCK_H
#define RTPMEDIACLOCK_H

#include <QObject>
#include <QElapsedTimer>
#include <QMutex>

class RtpMediaClock : public QObject
{
    Q_OBJECT

public:
    explicit RtpMediaClock(QObject *parent = nullptr);

    // İlk başlangıç
    void start(qint64 ptsUs);
    bool isStarted() const;
    // Yayın yeniden başlatılırsa
    void reset(qint64 ptsUs);

    qint64 currentTimeUs() const;

private:
    mutable QMutex m_mutex;

    QElapsedTimer m_timer;

    qint64 m_ptsBase = 0;
    bool m_started = false;

};

#endif

