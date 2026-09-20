#ifndef RTPPACKET_H
#define RTPPACKET_H

#include <QByteArray>
#include <QtGlobal>

struct RTPPacket
{
    QByteArray data;

    quint16 sequence = 0;

    quint32 timestamp = 0;

    quint32 ssrc = 0;

    bool marker = false;

    quint8 payloadType = 96;
    qint64 pts;   // MasterTime PTS
};

#endif
