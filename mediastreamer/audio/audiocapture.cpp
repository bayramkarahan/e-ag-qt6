#include "audiocapture.h"

#include <QDebug>

AudioCapture::AudioCapture(QObject *parent)
    : QObject(parent)
{
}

AudioCapture::~AudioCapture()
{
    close();
}

bool AudioCapture::open(int sampleRate,int channels)
{
    close();

    QAudioFormat format;
    format.setSampleRate(sampleRate);
    format.setChannelCount(channels);
    //format.setSampleRate(48000);
    //format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice device =
            QMediaDevices::defaultAudioInput();

    if(device.isNull())
    {
        qCritical() << "No audio input device found.";
        return false;
    }

    if(!device.isFormatSupported(format))
    {
        qCritical() << "Requested audio format is not supported.";
        return false;
    }

    m_source = new QAudioSource(device,
                                format,
                                this);

    m_source->setBufferSize(3840);   // 20 ms

    m_device = m_source->start();

    qDebug() << "Capture buffer =" << m_source->bufferSize();
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
qDebug() << "Capture buffer frames =" << m_source->bufferFrameCount();
#endif


    if(!m_device)
    {
        qCritical() << "Cannot start audio capture.";
        close();
        return false;
    }

    connect(m_device,
            &QIODevice::readyRead,
            this,
            &AudioCapture::readData);

    m_buffer.clear();
    m_pts = 0;

    qDebug() << "Audio capture started.";

    return true;
}

void AudioCapture::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
}

void AudioCapture::close()
{
    if(m_source)
    {
        m_source->stop();
        m_source->deleteLater();
        m_source = nullptr;
    }

    m_device = nullptr;

    m_buffer.clear();
    m_pts = 0;
}

void AudioCapture::readData()
{
    if(!m_device)
        return;

    QByteArray data = m_device->readAll();

    ///qDebug()<< "readAll() =" << data.size();

    m_buffer.append(data);



    static constexpr int FrameBytes = 960 * 2 * 2;
    // 960 sample * 2 kanal * 2 byte(Int16)
    while(m_buffer.size() >= FrameBytes)
    {
        AudioFrame frame;

        frame.data = m_buffer.left(FrameBytes);

        frame.sampleRate = 48000;
        frame.channels   = 2;
        frame.sampleSize = 16;

        if(m_masterTime){
            qint64 capturePts = m_masterTime->nowUs();
            frame.pts = capturePts;
           /* qDebug()
                << "AUDIO CAPTURE"
                << "pts =" << frame.pts
                << "thread =" << QThread::currentThread();*/
        }
        else
            frame.pts = 0;

       /// qDebug() << "AudioCapture CAPTURE AUDIO" << frame.pts;
        emit frameReady(frame);

        m_buffer.remove(0, FrameBytes);
    }

}
