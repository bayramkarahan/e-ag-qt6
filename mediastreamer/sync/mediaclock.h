#ifndef MEDIACLOCK_H
#define MEDIACLOCK_H

#include <QObject>
#include <QElapsedTimer>
#include <QMutex>

class MediaClock : public QObject
{
    Q_OBJECT

public:
    explicit MediaClock(QObject *parent = nullptr);

    // İlk başlangıç
    void start(qint64 ptsUs);
    bool isStarted() const;
    // Yayın yeniden başlatılırsa
    void reset(qint64 ptsUs);

    qint64 currentTimeUs() const;
    void adjust(qint64 ptsUs);
private:
    mutable QMutex m_mutex;

    QElapsedTimer m_timer;

    qint64 m_ptsBase = 0;
    bool m_started = false;

};

#endif

