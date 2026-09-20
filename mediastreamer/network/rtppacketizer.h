#ifndef RTPPACKETIZER_H
#define RTPPACKETIZER_H

#include <QObject>

#include "../common/EncodedPacket.h"
#include "rtppacket.h"
#include <QDebug>
class RTPPacketizer : public QObject
{
    Q_OBJECT

public:

    explicit RTPPacketizer(QObject *parent = nullptr);

public slots:

    void packetize(const EncodedPacket &packet);

signals:

    void packetReady(const RTPPacket &packet);

private:

    QByteArray createHeader(bool marker,
                            quint8 payloadType,
                            quint16 sequence,
                            quint32 timestamp,
                            quint32 ssrc);

private:

    quint16 m_sequence = 0;

    quint32 m_ssrc = 0;

    quint32 m_videoTimestamp = 0;

    quint32 m_audioTimestamp = 0;
};

#endif
