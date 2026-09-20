#ifndef VIDEOFRAMECONVERTER_H
#define VIDEOFRAMECONVERTER_H

#include "../common/MediaFrame.h"
extern "C"
{
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
#include <libavutil/imgutils.h>
}
#include <libswscale/swscale.h>
class VideoFrameConverter
{

public:

    VideoFrameConverter();

    bool convert(const MediaFrame &input,
                 MediaFrame &output);


private:

    bool rgbaToYuv420(const MediaFrame &input,
                      MediaFrame &output);
private:

    bool convertSwScale(const MediaFrame &input,
                        MediaFrame &output,
                        AVPixelFormat srcFormat);
};


#endif
