#ifndef AUDIOOPUSENCODER_H
#define AUDIOOPUSENCODER_H

#include <QObject>
#include <opus/opus.h>

#include "../audio/audioframe.h"
#include "../common/EncodedPacket.h"

class AudioOpusEncoder : public QObject
{
    Q_OBJECT

public:
    explicit AudioOpusEncoder(QObject *parent = nullptr);
    ~AudioOpusEncoder();

    bool open(int sampleRate = 48000,
              int channels = 2,
              int bitrate = 64000);

    void close();

public slots:
    void encode(const AudioFrame &frame);

signals:
    void packetReady(const EncodedPacket &packet);

private:
    OpusEncoder *m_encoder = nullptr;

    int m_sampleRate = 48000;
    int m_channels   = 2;

};

#endif // AUDIOOPUSENCODER_H
