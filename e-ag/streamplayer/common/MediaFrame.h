#ifndef MEDIAFRAME_H
#define MEDIAFRAME_H

#include <QtGlobal>
#include "MediaBuffer.h"
#include <QSharedPointer>
class HardwareFrame;
enum class PixelFormat
{
    Unknown,

    NV12,

    YUV420P,

    RGBA8888,

    YUYV,

    UYVY,

    RGB24
};

struct MediaFrame
{
    bool isHardwareFrame() const
    {
        return !hardwareFrame.isNull();
    }

    bool isCpuFrame() const
    {
        return !buffer.isNull();
    }
    QSharedPointer<MediaBuffer> buffer;
    QSharedPointer<HardwareFrame> hardwareFrame;
    int width = 0;

    int height = 0;

    PixelFormat format = PixelFormat::Unknown;

    qint64 pts = 0;

    bool keyFrame = false;
    uint64_t sequence;
    int bufferIndex = -1;

    int bufferBytesUsed = 0;
};

#endif
