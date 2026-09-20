#include "mediaaudiothread.h"

#include <QElapsedTimer>
#include <QThread>

MediaAudioThread::MediaAudioThread(MediaFileCapture *capture,
                                   QObject *parent)
    : QObject(parent),
      m_capture(capture)
{
    moveToThread(&m_thread);

    connect(&m_thread,
            &QThread::started,
            this,
            &MediaAudioThread::process);
}

void MediaAudioThread::start()
{
    if (m_running)
        return;

    m_running = true;
    m_thread.start();
}

void MediaAudioThread::stop()
{
    m_running = false;

    m_thread.quit();
    m_thread.wait();
}

void MediaAudioThread::process()
{
   /* qDebug()
        << "process thread =" << QThread::currentThread()
        << "object thread =" << this->thread();*/

    QElapsedTimer localClock;
    qint64 firstPts = -1;

    while (m_running)
    {
        AudioFrame frame;

        if (!m_capture->grabAudio(frame))
        {
            QThread::msleep(1);
            continue;
        }

        if (m_settings.timeMode == TimeMode::FramePts)
        {
            if (firstPts < 0)
            {
                firstPts = frame.pts;
                localClock.start();
            }

            qint64 targetUs  = frame.pts - firstPts;
            qint64 elapsedUs = localClock.nsecsElapsed() / 1000;

            if (targetUs > elapsedUs)
                QThread::usleep(targetUs - elapsedUs);
        }
       /* qDebug()
             << "MEDIA AUDIO THREAD"
             << frame.sampleRate
             << frame.channels
             << frame.data.size()
             << frame.pts;*/
        emit frameReady(frame);
    }
}
