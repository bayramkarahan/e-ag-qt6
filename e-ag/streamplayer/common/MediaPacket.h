#ifndef MEDIAPACKET_H
#define MEDIAPACKET_H

#include <QByteArray>

enum class PacketType
{
    Video,

    Audio
};

struct MediaPacket
{
    QByteArray data;

    PacketType type;

    bool keyFrame=false;

    qint64 pts=0;
};

#endif
