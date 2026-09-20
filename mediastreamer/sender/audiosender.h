#ifndef AUDIOSENDER_H
#define AUDIOSENDER_H

#include <QObject>
#include <QHostAddress>

#include "audio/audiocapture.h"
#include "encoder/audioopusencoder.h"
#include "network/rtppacketizer.h"
#include "sender/rtpsender.h"
#include<QThread>
#include "sync/mastertime.h"
struct AudioClient
{
    QHostAddress address;
    quint16 port;
};
class AudioSender : public QObject
{
    Q_OBJECT

public:
    explicit AudioSender(QObject *parent = nullptr);
    bool open(int sampleRate,
              int channels,
              int bitrate,
              const QHostAddress &group,
              quint16 port);
    QList<AudioClient> m_clients;
    void close();

    void setMasterTime(MasterTime *masterTime);

public slots:
    void onFrame(const AudioFrame &frame);
    void addClient(const QHostAddress &addr, quint16 port);
    void removeClient(const QHostAddress &addr, quint16 port);
    void clearClients();
private:
    AudioOpusEncoder m_encoder;
    RTPPacketizer    m_packetizer;
    RtpSender  m_sender;
    MasterTime *m_masterTime = nullptr;
};

#endif // AUDIOSENDER_H
