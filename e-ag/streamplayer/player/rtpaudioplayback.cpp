#include "rtpaudioplayback.h"

#include <QDebug>


RtpAudioPlayback::RtpAudioPlayback(QObject *parent)
    : QObject(parent)
{
}


RtpAudioPlayback::~RtpAudioPlayback()
{
    close();
}


bool RtpAudioPlayback::open(int sampleRate,
                         int channels)
{
    close();

    m_format.setSampleRate(sampleRate);
    m_format.setChannelCount(channels);
    m_format.setSampleFormat(
                QAudioFormat::Int16);


    QAudioDevice device =
            QMediaDevices::defaultAudioOutput();


    if(!device.isFormatSupported(m_format))
    {
        qWarning()
            << "Audio format not supported.";

        return false;
    }

    m_sink = new QAudioSink(device,
                            m_format,
                            this);

    // Buffer'ı küçült
    //m_sink->setBufferSize(960);
    // Eğer Qt sürümünde yoksa bunun yerine:
    m_sink->setBufferSize(3840);



    m_device = m_sink->start();

    qDebug() << "Actual buffer bytes =" << m_sink->bufferSize();

    #if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
    qDebug() << "Actual buffer frames =" << m_sink->bufferFrameCount();
    #endif


    if(!m_device)
    {
        qWarning()
            << "Cannot start audio output.";

        close();

        return false;
    }


    qDebug()
        << "Audio playback started.";

    return true;
}


void RtpAudioPlayback::close()
{
    if(m_sink)
    {
        m_sink->stop();
        delete m_sink;
        m_sink = nullptr;
    }

    m_device = nullptr;
}


void RtpAudioPlayback::onFrame(const AudioFrame &frame)
{
    if(!m_device)
        return;


    qint64 written = m_device->write(frame.data);
   /* qDebug() << "AUDIO PLAY"
             << "pts =" << frame.pts
             << "time =" << QDateTime::currentMSecsSinceEpoch();
*/

    if (written <= 0)
    {
        qDebug() << "AUDIO WRITE FAILED";
    }
     if(written > 0)
     {

       // m_clock->adjust(frame.pts);

     }
}


void RtpAudioPlayback::setClock(RtpMediaClock *clock)
{
    m_clock = clock;
}
