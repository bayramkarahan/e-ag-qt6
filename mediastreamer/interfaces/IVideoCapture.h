#ifndef IVIDEOCAPTURE_H
#define IVIDEOCAPTURE_H

#include "../common/VideoCaptureSettings.h"
#include "common/MediaFrame.h"
#include "../sync/mastertime.h"
#include "../audio/audioframe.h"
class IVideoCapture
{
public:
    virtual ~IVideoCapture() = default;

    virtual bool open(const VideoCaptureSettings &settings) = 0;

    virtual void close() = 0;

    virtual bool start() = 0;

    virtual void stop() = 0;

    virtual bool grab(MediaFrame &frame) = 0;

    virtual void release(MediaFrame &frame) = 0;

    virtual void setMasterTime(MasterTime *masterTime) = 0;
    virtual bool grabAudio(AudioFrame &frame)
    {
        Q_UNUSED(frame)
        return false;
    }
};

#endif
