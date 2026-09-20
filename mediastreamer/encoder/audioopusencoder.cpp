#include "audioopusencoder.h"

#include <QDebug>

AudioOpusEncoder::AudioOpusEncoder(QObject *parent)
    : QObject(parent)
{
}

AudioOpusEncoder::~AudioOpusEncoder()
{
    close();
}

bool AudioOpusEncoder::open(int sampleRate,
                            int channels,
                            int bitrate)
{
    close();

    m_sampleRate = sampleRate;
    m_channels   = channels;
    qCritical() << "sampleRate:"<<sampleRate;
    qCritical() << "channels:"<<channels;
    qCritical() << "bitrate:"<<bitrate;

    int error = OPUS_OK;

    m_encoder = opus_encoder_create(sampleRate,
                                    channels,
                                    OPUS_APPLICATION_AUDIO,
                                    &error);

    if(error != OPUS_OK || !m_encoder)
    {
        qCritical() << "Cannot create Opus encoder:"
                    << opus_strerror(error);
        return false;
    }

    opus_encoder_ctl(m_encoder,
                     OPUS_SET_BITRATE(bitrate));

    opus_encoder_ctl(m_encoder,
                     OPUS_SET_COMPLEXITY(5));

    opus_encoder_ctl(m_encoder,
                     OPUS_SET_VBR(1));

    opus_encoder_ctl(m_encoder,
                     OPUS_SET_INBAND_FEC(0));

    opus_encoder_ctl(m_encoder,
                     OPUS_SET_DTX(0));

    qDebug() << "Opus encoder initialized.";

    return true;
}

void AudioOpusEncoder::close()
{
    if(m_encoder)
    {
        opus_encoder_destroy(m_encoder);
        m_encoder = nullptr;
    }
}

void AudioOpusEncoder::encode(const AudioFrame &frame)
{
    ///qDebug() << "AudioOpusEncoder ENCODE START";

    if(!m_encoder)
    {
        qDebug() << "Encoder is null";
        return;
    }

    if(frame.data.isEmpty())
    {
        qDebug() << "Frame data empty";
        return;
    }




    const opus_int16 *pcm =
        reinterpret_cast<const opus_int16*>(frame.data.constData());


    int frameSamples =
        frame.data.size() /
        (sizeof(opus_int16) * m_channels);
   /* qDebug()
        << "OPUS ENCODE INPUT"
        << "pts =" << frame.pts
        << "samples =" <<  frameSamples
        << "wall =" << QDateTime::currentMSecsSinceEpoch();*/
   /* qDebug()
        << "Calculated frameSamples =" << frameSamples;*/

    if(frameSamples != 960)
    {
        /*qDebug()
            << "Frame rejected. Expected 960 samples but got"
            << frameSamples;*/

        return;
    }

    QByteArray encoded;
    encoded.resize(1500);

    int bytes = opus_encode(
        m_encoder,
        pcm,
        frameSamples,
        reinterpret_cast<unsigned char*>(encoded.data()),
        encoded.size());

   /* qDebug()
        << "opus_encode returned"
        << bytes;*/

    if(bytes < 0)
    {
        qWarning()
            << "opus_encode failed:"
            << opus_strerror(bytes);

        return;
    }

    encoded.resize(bytes);

    EncodedPacket packet;

    packet.codec      = CodecType::Opus;
    packet.pts        = frame.pts;
    packet.dts        = frame.pts;
    //packet.duration   = 20000;
    packet.duration   = (1000000LL * frameSamples) / m_sampleRate;
    packet.keyFrame   = true;
    packet.config     = false;
    packet.endOfFrame = true;
    packet.data       = std::move(encoded);

   /* qDebug()
        << "OPUS ENCODE OUTPUT"
        << "pts =" << packet.pts
        << "duration =" << packet.duration
        << "size =" << packet.data.size()
        << "wall =" << QDateTime::currentMSecsSinceEpoch();*/

///qDebug() << "ENCODE SEND AUDIO" << frame.pts;
    emit packetReady(packet);

   /// qDebug() << "packetReady emitted";
}
