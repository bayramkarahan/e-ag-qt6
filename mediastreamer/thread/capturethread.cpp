#include "capturethread.h"

#include <QMetaObject>
#include <QDebug>

CaptureThread::CaptureThread(QObject *parent)
    : QObject(parent)
{
    moveToThread(&m_thread);

    connect(&m_thread,
            &QThread::started,
            this,
            &CaptureThread::process,
            Qt::QueuedConnection);
}

CaptureThread::~CaptureThread()
{
    stop();
}

bool CaptureThread::open(const VideoCaptureSettings &settings)
{
    if(m_capture)
    {
        delete m_capture;
        m_capture = nullptr;
    }
    m_settings=settings;

    m_mediaFileCapture = nullptr;
    switch(settings.source)
    {
    case CaptureSource::Camera:
        m_capture = new V4L2Capture;
        break;

    case CaptureSource::ScreenQt:
        m_capture = new ScreenCapture;
        break;

   case CaptureSource::ScreenFFmpeg:
    {
        m_capture = new FFmpegScreenCapture;
        qDebug()<<"FFmpegScreenCapture:"<<int(settings.source);
        break;
}
    case CaptureSource::MediaFile:
        {//m_capture = new MediaFileCapture;
        auto capture = new MediaFileCapture;
                m_capture = capture;
                m_mediaFileCapture = capture;
        break;
}
    case CaptureSource::ScreenFFmpegPipeWire:
        m_capture = new FFmpegPipeWireScreenCapture;

        break;
    }



    if(!m_capture)
        return false;

    return m_capture->open(settings);
}

void CaptureThread::setMasterTime(MasterTime *masterTime)
{
    m_capture->setMasterTime(masterTime);
}

void CaptureThread::start()
{
    if (m_running.loadAcquire())
        return;

    if (!m_capture || !m_capture->start())
    {
        emit error("Camera streaming could not be started.");
        return;
    }

    //
    // Sadece medya dosyasında reader thread çalıştır.
    //
    if (m_mediaFileCapture)
    {
        m_reader = new MediaReaderThread;

        m_reader->setCapture(m_mediaFileCapture);

        m_reader->start();
    }

    m_running.storeRelease(true);

    m_thread.start();
}

void CaptureThread::stop()
{
    m_running.storeRelease(false);

    if (m_reader)
    {
        m_reader->stop();
        delete m_reader;
        m_reader = nullptr;
    }

    m_thread.quit();
    m_thread.wait();

    if (m_capture)
        m_capture->stop();
}

void CaptureThread::process()
{
    ///qDebug() << "PROCESS START";
    QElapsedTimer localClock;
    qint64 firstPts = -1;

    while (m_running.loadAcquire())
    {
        static bool first = true;

        if (first)
        {
            first = false;
            qDebug() << "FIRST grab()";
        }
        //-------------------------------------------------
        // Kamera / ekran
        //-------------------------------------------------
        if (!m_mediaFileCapture)
        {
            MediaFrame frame;

            if (m_capture->grab(frame))
            {
               // qDebug()<< "CAPTURE VIDEO"<< frame.pts;

                emit frameReady(frame);
            }

            continue;
        }

        //-------------------------------------------------
        // Queue boşsa ReaderThread'in doldurmasını bekle
        //-------------------------------------------------

        if (!m_mediaFileCapture->hasVideoFrame() &&
            !m_mediaFileCapture->hasAudioFrame())
        {
            QThread::msleep(1);
            continue;
        }

        //-------------------------------------------------
        // Hangisi önce gönderilecek?
        //-------------------------------------------------

        bool sendVideo;

        if (!m_mediaFileCapture->hasAudioFrame())
        {
            sendVideo = true;
        }
        else if (!m_mediaFileCapture->hasVideoFrame())
        {
            sendVideo = false;
        }
        else
        {
            sendVideo =
                m_mediaFileCapture->videoPts() <=
                m_mediaFileCapture->audioPts();
        }

        //-------------------------------------------------
        // VIDEO
        //-------------------------------------------------

        if (sendVideo)
        {
            MediaFrame video;

            if (!m_mediaFileCapture->grab(video))
                continue;

            if (m_settings.timeMode == TimeMode::FramePts)
            {
                if (firstPts < 0)
                {
                    firstPts = video.pts;
                    localClock.start();

                    qDebug() << "FIRST VIDEO =" << firstPts;
                }

                qint64 targetUs = video.pts - firstPts;
                qint64 elapsedUs = localClock.nsecsElapsed() / 1000;

                if (targetUs > elapsedUs)
                {
                    ///qDebug() << "VIDEO WAIT" << targetUs - elapsedUs;

                    QThread::usleep(targetUs - elapsedUs);
                }
            }

            //qDebug() << "SEND VIDEO" << video.pts;

            emit frameReady(video);
        }

        //-------------------------------------------------
        // AUDIO
        //-------------------------------------------------

        else
        {
            AudioFrame audio;

            if (!m_mediaFileCapture->grabAudio(audio))
                continue;

           /* qDebug() << "SEND AUDIO" << audio.pts;*/

            emit audioFrameReady(audio);
        }
    }
}

