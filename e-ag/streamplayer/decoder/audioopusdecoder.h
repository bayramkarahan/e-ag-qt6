#ifndef AUDIOOPUSDECODER_H
#define AUDIOOPUSDECODER_H

#include <QObject>
#include <opus/opus.h>

#include "../network/rtppacket.h"
#include "../audio/audioframe.h"

class AudioOpusDecoder : public QObject
{
    Q_OBJECT

public:
    explicit AudioOpusDecoder(QObject *parent = nullptr);
    ~AudioOpusDecoder();

    bool open(int sampleRate = 48000,
              int channels = 2);

    void close();

public slots:
    void decodeRtp(const RTPPacket &packet);
    void decodeRstp(const QByteArray &data,qint64 pts);
signals:
    void frameReady(const AudioFrame &frame);

private:
    OpusDecoder *m_decoder = nullptr;

  qint64  m_firstRtpTimestamp = 0;
  bool    m_haveFirstRtpTimestamp = false;
    int m_sampleRate = 48000;
    int m_channels = 2;
    bool m_started = false;
};

#endif
