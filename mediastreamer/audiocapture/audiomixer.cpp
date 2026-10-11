
#include "audiomixer.h"

#include <QtEndian>
#include <QtGlobal>

#include <algorithm>

AudioMixer::AudioMixer(QObject *parent)
    : QObject(parent)
{
}

void AudioMixer::onMicrophoneFrame(const AudioFrame &frame)
{
    m_microphoneQueue.enqueue(frame);

    // Geciken kaynağın kuyruğu sınırsız büyümesin.
    while (m_microphoneQueue.size() > MaxQueueSize)
        m_microphoneQueue.dequeue();

    mixAvailableFrames();
}

void AudioMixer::onSystemFrame(const AudioFrame &frame)
{
    m_systemQueue.enqueue(frame);

    while (m_systemQueue.size() > MaxQueueSize)
        m_systemQueue.dequeue();

    mixAvailableFrames();
}

void AudioMixer::mixAvailableFrames()
{
    while (!m_microphoneQueue.isEmpty() &&
           !m_systemQueue.isEmpty()) {

        const AudioFrame mic = m_microphoneQueue.dequeue();
        const AudioFrame system = m_systemQueue.dequeue();

        if (mic.sampleRate != system.sampleRate ||
            mic.channels != system.channels ||
            mic.sampleSize != 16 ||
            system.sampleSize != 16) {
            qWarning("AudioMixer: Audio formats do not match.");
            continue;
        }

        AudioFrame mixed = mixFrames(mic, system);

        if (!mixed.data.isEmpty())
            emit frameReady(mixed);
    }
}

AudioFrame AudioMixer::mixFrames(const AudioFrame &mic,
                                 const AudioFrame &system)
{
    AudioFrame output;

    output.sampleRate = mic.sampleRate;
    output.channels = mic.channels;
    output.sampleSize = 16;

    // Yalnızca iki kaynakta da bulunan örnekleri birleştir.
    const qsizetype byteCount =
        std::min(mic.data.size(), system.data.size()) & ~qsizetype(1);

    if (byteCount <= 0)
        return output;

    output.data.resize(byteCount);

    const auto *micData =
        reinterpret_cast<const uchar *>(mic.data.constData());

    const auto *systemData =
        reinterpret_cast<const uchar *>(system.data.constData());

    auto *outData =
        reinterpret_cast<uchar *>(output.data.data());

    for (qsizetype i = 0; i < byteCount; i += 2) {
        const qint16 micSample =
            qFromLittleEndian<qint16>(micData + i);

        const qint16 systemSample =
            qFromLittleEndian<qint16>(systemData + i);

        // Her iki ses kaynağını topla.
        const int sum =
            static_cast<int>(micSample) +
            static_cast<int>(systemSample);

        const qint16 mixedSample = static_cast<qint16>(
            qBound(-32768, sum, 32767));

        qToLittleEndian<qint16>(mixedSample, outData + i);
    }

    // İki kaynağın zaman damgalarının ortalaması.
    output.pts = mic.pts + (system.pts - mic.pts) / 2;

    return output;
}
