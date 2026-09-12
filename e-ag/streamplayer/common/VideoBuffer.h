#ifndef VIDEOBUFFER_H
#define VIDEOBUFFER_H

#include <QtGlobal>

enum class PixelFormat
{
    Unknown,

    NV12,

    YUV420P,

    YUYV,

    UYVY,

    RGB24
};

class VideoBuffer
{
public:

    uchar* plane[4]={nullptr,nullptr,nullptr,nullptr};

    int stride[4]={0,0,0,0};

    int planeSize[4]={0,0,0,0};

    int width=0;

    int height=0;

    PixelFormat format=PixelFormat::Unknown;
int bytesPerLine = 0;
    qint64 pts=0;

    void* privateData=nullptr;
    bool isValid() const
    {
        return planeCount > 0 &&
               plane[0] != nullptr;
    }
};

#endif
