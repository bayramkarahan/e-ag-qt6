#include "mediareaderthread.h"
#include "capture/mediafilecapture.h"

#include <QThread>

MediaReaderThread::MediaReaderThread(QObject *parent)
    : QThread(parent)
{
}

void MediaReaderThread::setCapture(MediaFileCapture *capture)
{
    m_capture = capture;
}

void MediaReaderThread::stop()
{
    m_running.storeRelease(false);
    wait();
}

void MediaReaderThread::run()
{
    if (!m_capture)
        return;

    m_running.storeRelease(true);

    while (m_running.loadAcquire())
    {
        if (!m_capture->readNextPacket())
            break;
    }
}
