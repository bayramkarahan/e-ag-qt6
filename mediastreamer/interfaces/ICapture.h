#ifndef ICAPTURE_H
#define ICAPTURE_H
#include<qstring.h>
#include "../common/MediaFrame.h"
#include "../common/StreamSettings.h"

class ICapture
{
public:

    virtual ~ICapture() = default;
    virtual bool open(const VideoCaptureSettings&)=0;

    //virtual bool open() = 0;

    virtual bool start() = 0;

    virtual void stop() = 0;

    virtual void close() = 0;

    virtual bool grab(MediaFrame &frame) = 0;

    //virtual void release() = 0;
    virtual void release(MediaFrame &frame)=0;
};

#endif
