#include "audioprocessor.h"

AudioProcessor::AudioProcessor(QObject *parent)
    : QObject(parent)
{
}

void AudioProcessor::setFrameSamples(int samples)
{
    m_frameSamples = samples;
}

void AudioProcessor::setFormat(int sampleRate,
                               int channels,
                               int sampleSize)
{
    m_sampleRate = sampleRate;
    m_channels = channels;
    m_sampleSize = sampleSize;
}

void AudioProcessor::onFrame(const AudioFrame &frame)
{
    if(frame.data.isEmpty())
        return;

    if(frame.sampleRate != m_sampleRate)
        m_sampleRate = frame.sampleRate;

    if(frame.channels != m_channels)
        m_channels = frame.channels;

    if(m_buffer.isEmpty())
        m_bufferPts = frame.pts;

    m_buffer.append(frame.data);

    const int bytesPerSample =
            (m_sampleSize / 8) * m_channels;

    const int targetBytes =
            m_frameSamples * bytesPerSample;

    while(m_buffer.size() >= targetBytes)
    {
        AudioFrame out;

        out.sampleRate = m_sampleRate;
        out.channels   = m_channels;
        out.sampleSize = m_sampleSize;

        out.pts = m_bufferPts;

        QByteArray copy(m_buffer.constData(), targetBytes);
        out.data = std::move(copy);

        m_buffer.remove(0,targetBytes);

        const qint64 durationUs =
                (1000000LL * m_frameSamples) / m_sampleRate;

        m_bufferPts += durationUs;
            ///qDebug() << "AudioProcessor CAPTURE AUDIO" << out.pts;
        emit frameReady(out);
    }
}

/*
void AudioProcessor::onFrame(const AudioFrame &frame)
{
    /*qDebug()
        << "processor thread =" << QThread::currentThread()
        << "object thread =" << this->thread();
*/
/*
    if(frame.data.isEmpty())
        return;

    if(frame.sampleRate != m_sampleRate)
        m_sampleRate = frame.sampleRate;

    if(frame.channels != m_channels)
        m_channels = frame.channels;

    if (m_buffer.isEmpty())
        m_bufferPts = frame.pts;

    m_buffer.append(frame.data);

    const int bytesPerSample =
            (m_sampleSize / 8) * m_channels;

    const int targetBytes =
            m_frameSamples * bytesPerSample;

    while(m_buffer.size() >= targetBytes)
    {
       /* AudioFrame out;

        out.sampleRate = m_sampleRate;
        out.channels   = m_channels;
        out.sampleSize = m_sampleSize;

        //out.data = m_buffer.left(targetBytes);

       // m_buffer.remove(0,targetBytes);
        out.data = m_buffer.first(targetBytes);
        out.data.detach();

        m_buffer.remove(0,targetBytes);
        emit frameReady(out);*/
  /*      AudioFrame out;

        out.sampleRate = m_sampleRate;
        out.channels   = m_channels;
        out.sampleSize = m_sampleSize;
        out.pts=frame.pts;
        //m_buffer.append(frame.data);
        /*qint64 durationUs =
            (1000000LL * m_frameSamples) / m_sampleRate;

        m_bufferPts += durationUs;*/
/*
        QByteArray copy(m_buffer.constData(), targetBytes);

        out.data = std::move(copy);

        m_buffer.remove(0,targetBytes);

        emit frameReady(out);
    }
}*/
