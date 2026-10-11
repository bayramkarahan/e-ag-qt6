#ifndef V4L2BUFFER_H
#define V4L2BUFFER_H

#include "../common/MediaBuffer.h"

class V4L2Capture;

class V4L2Buffer : public MediaBuffer
{
public:

    V4L2Buffer() = default;
    ~V4L2Buffer() override = default;

    int index = -1;
};

#endif
