#include "audioopusdecoder.h"

#include <QDebug>

AudioOpusDecoder::AudioOpusDecoder(QObject *parent)
    : QObject(parent)
{
}

AudioOpusDecoder::~AudioOpusDecoder()
{
    close();
}

bool AudioOpusDecoder::open(int sampleRate,
                            int channels)
{
    close();

    m_sampleRate = sampleRate;
    m_channels   = channels;

    int error = OPUS_OK;

    m_decoder = opus_decoder_create(sampleRate,
                                    channels,
                                    &error);

    if(error != OPUS_OK || !m_decoder)
    {
        qCritical() << "Cannot create Opus decoder:"
                    << opus_strerror(error);

        return false;
    }
   // m_firstRtpTimestamp = 0;
   //  m_haveFirstRtpTimestamp = false;
    qDebug() << "Opus decoder initialized.";

    return true;
}


void AudioOpusDecoder::close()
{
    if(m_decoder)
    {
        opus_decoder_destroy(m_decoder);
        m_decoder = nullptr;
    }
}

void AudioOpusDecoder::decodeRstp(const QByteArray &data,
                              qint64 pts)
{
    if (!m_decoder || data.isEmpty())
        return;

    const int maxSamples = 960;

    QByteArray pcm;
    pcm.resize(maxSamples * m_channels * sizeof(opus_int16));

    opus_int16 *output =
            reinterpret_cast<opus_int16*>(pcm.data());

    int samples =
        opus_decode(
            m_decoder,
            reinterpret_cast<const unsigned char*>(data.constData()),
            data.size(),
            output,
            maxSamples,
            0);

    if (samples < 0)
        return;

    pcm.resize(samples * m_channels * sizeof(opus_int16));

    AudioFrame frame;

    frame.data = std::move(pcm);
    frame.sampleRate = m_sampleRate;
    frame.channels = m_channels;
    frame.sampleSize = 16;
    frame.pts = pts;

    emit frameReady(frame);
}


/*void AudioOpusDecoder::decodeRtp(const RTPPacket &packet)
{
    if (!m_decoder)
        return;

    if (packet.data.isEmpty())
        return;

    const int maxSamples = 960;

    QByteArray pcm;
    pcm.resize(maxSamples *
               m_channels *
               sizeof(opus_int16));

    opus_int16 *output =
        reinterpret_cast<opus_int16 *>(pcm.data());

    int samples = opus_decode(
        m_decoder,
        reinterpret_cast<const unsigned char *>(
            packet.data.constData()),
        packet.data.size(),
        output,
        maxSamples,
        0);

    if (samples < 0)
    {
        qWarning()
            << "opus_decode failed:"
            << samples
            << opus_strerror(samples);

        return;
    }

    pcm.resize(samples *
               m_channels *
               sizeof(opus_int16));

    AudioFrame frame;

    frame.data = std::move(pcm);
    frame.sampleRate = m_sampleRate;
    frame.channels = m_channels;
    frame.sampleSize = 16;

    frame.pts =
        (qint64(packet.timestamp) * 1000000LL)
        / 48000LL;
frame.pts = 0;
   /* static int count = 0;
    ++count;

    if (count % 100 == 0)
    {
        qDebug()
            << "OPUS DECODE:"
            << "count =" << count
            << "samples =" << samples
            << "pts =" << frame.pts
            << "bytes =" << frame.data.size();
    }*/
/*
    emit frameReady(frame);
}*/
void AudioOpusDecoder::decodeRtp(const RTPPacket &packet)
{
    if (!m_decoder)
        return;

    if (packet.data.isEmpty())
        return;
/*
    if (!m_haveFirstRtpTimestamp)
    {
        m_firstRtpTimestamp = packet.timestamp;
        m_haveFirstRtpTimestamp = true;

        qDebug() << "OPUS FIRST RTP TIMESTAMP ="
                 << m_firstRtpTimestamp;
    }

    quint32 relativeTimestamp =
        packet.timestamp - m_firstRtpTimestamp;

    qint64 pts =
        (qint64(relativeTimestamp) * 1000000LL) / 48000LL;
*/
    // Göndericide RTP timestamp, packet.pts üzerinden üretiliyor.
    // Video ile aynı kaynak zaman çizelgesini kullan.
    qint64 pts =
        (static_cast<qint64>(packet.timestamp) * 1000000LL)
        / 48000LL;

   /* qDebug() << "OPUS RTP:"
             << "timestamp =" << packet.timestamp
             << "first =" << m_firstRtpTimestamp
             << "relative =" << relativeTimestamp
             << "PTS =" << pts;*/

    const int maxSamples = 960;

    QByteArray pcm;
    pcm.resize(
        maxSamples *
        m_channels *
        sizeof(opus_int16));

    opus_int16 *output =
        reinterpret_cast<opus_int16*>(pcm.data());

    int samples =
        opus_decode(
            m_decoder,
            reinterpret_cast<const unsigned char*>(
                packet.data.constData()),
            packet.data.size(),
            output,
            maxSamples,
            0);

    if (samples < 0)
    {
        qWarning()
            << "opus_decode failed:"
            << opus_strerror(samples);

        return;
    }

    pcm.resize(
        samples *
        m_channels *
        sizeof(opus_int16));

    AudioFrame frame;

    frame.data = std::move(pcm);
    frame.sampleRate = m_sampleRate;
    frame.channels = m_channels;
    frame.sampleSize = 16;
    frame.pts = pts;

    emit frameReady(frame);
}

/*
void AudioOpusDecoder::decodeRtp(const RTPPacket &packet)
{
    if(!m_decoder)
        return;

    if(packet.data.isEmpty())
        return;


    // 20 ms @ 48kHz = 960 sample/channel
    const int maxSamples = 960;


    QByteArray pcm;

    pcm.resize(maxSamples *
               m_channels *
               sizeof(opus_int16));


    opus_int16 *output =
        reinterpret_cast<opus_int16*>(pcm.data());


    int samples = opus_decode(
                m_decoder,
                reinterpret_cast<const unsigned char*>(
                    packet.data.constData()),
                packet.data.size(),
                output,
                maxSamples,
                0);


    if(samples < 0)
    {
        qWarning() << "opus_decode failed:"
                   << opus_strerror(samples);
        return;
    }


    pcm.resize(samples *
               m_channels *
               sizeof(opus_int16));


    AudioFrame frame;

    frame.data = std::move(pcm);

    frame.sampleRate = m_sampleRate;
    frame.channels   = m_channels;
    frame.sampleSize = 16;

    // RTP timestamp şimdilik pts olarak kullanılıyor
    //
    frame.pts =
        (qint64(packet.timestamp) * 1000000LL) / 48000LL;
//frame.pts = packet.timestamp;
   /* qDebug()
        << "AudioOpusDecoder RTP seq"
        << packet.sequence
        << "pts"
        << frame.pts;*/
   /* emit frameReady(frame);
}*/
/*
void AudioOpusDecoder::decodeRtp(const RTPPacket &packet)
{
    if(!m_decoder)
        return;

    if(packet.data.isEmpty())
        return;


    // 20 ms @ 48kHz = 960 sample/channel
    const int maxSamples = 960;


    QByteArray pcm;

    pcm.resize(maxSamples *
               m_channels *
               sizeof(opus_int16));


    opus_int16 *output =
        reinterpret_cast<opus_int16*>(pcm.data());


    int samples = opus_decode(
                m_decoder,
                reinterpret_cast<const unsigned char*>(
                    packet.data.constData()),
                packet.data.size(),
                output,
                maxSamples,
                0);


    if(samples < 0)
    {
        qWarning() << "opus_decode failed:"
                   << opus_strerror(samples);
        return;
    }


    pcm.resize(samples *
               m_channels *
               sizeof(opus_int16));


    AudioFrame frame;

    frame.data = std::move(pcm);

    frame.sampleRate = m_sampleRate;
    frame.channels   = m_channels;
    frame.sampleSize = 16;

    // RTP timestamp şimdilik pts olarak kullanılıyor
    //frame.pts = packet.timestamp;
    frame.pts =
        (qint64(packet.timestamp) * 1000000LL) / 48000LL;

    /*qDebug()
        << "AudioOpusDecoder RTP seq"
        << packet.sequence
        << "pts"
        << frame.pts;*/
  /*  emit frameReady(frame);
}*/

