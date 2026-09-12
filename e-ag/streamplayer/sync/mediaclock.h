#ifndef MEDIACLOCK_H
#define MEDIACLOCK_H

#include <QObject>
#include <QElapsedTimer>
#include <QMutex>
#include<QDebug>
class MediaClock : public QObject
{
    Q_OBJECT

public:
    explicit MediaClock(QObject *parent = nullptr);

    // İlk başlangıç
    void start(qint64 ptsUs);
    void restart(qint64 ptsUs);
    bool isStarted() const;
    // Yayın yeniden başlatılırsa
    void reset(qint64 ptsUs);

    qint64 currentTimeUs() const;
    void adjust(qint64 ptsUs);

    void setVideoStarted(bool started)
       {
           m_videoStarted = started;
       }

       bool videoStarted() const
       {
           return m_videoStarted;
       }

    bool registerVideoFirstFrame(qint64 ptsUs);
    bool registerAudioFirstFrame(qint64 ptsUs);

    bool bothStreamsReady() const;

    void startFromStreams();

   private:
    bool m_videoStarted = false;
    mutable QMutex m_mutex;

    QElapsedTimer m_timer;

    qint64 m_ptsBase = 0;
    bool m_started = false;

    bool m_videoFirstReceived = false;
    bool m_audioFirstReceived = false;

    qint64 m_firstVideoPts = -1;
    qint64 m_firstAudioPts = -1;

};

#endif

