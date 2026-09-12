#ifndef ENCODEDPACKET_H
#define ENCODEDPACKET_H

#include <QByteArray>

enum class CodecType
{
    Unknown,

    H264,

    Opus
};

struct EncodedPacket
{
    QByteArray data;

    CodecType codec = CodecType::Unknown;

    qint64 pts = 0;
    qint64 dts = 0;

    int duration = 0;

    bool keyFrame = false;
    bool config = false;

    bool endOfFrame = false;
};

#endif
