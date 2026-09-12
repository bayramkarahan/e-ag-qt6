#ifndef VAAPIHARDWAREFRAME_H
#define VAAPIHARDWAREFRAME_H
#pragma once

#include "hardwareframe.h"

extern "C"
{
#include <libavutil/frame.h>
}
extern "C" {
#include <va/va.h>
}
class VaapiHardwareFrame : public HardwareFrame
{
public:

    VaapiHardwareFrame() = default;

    ~VaapiHardwareFrame() override
    {
        if(frame)
            av_frame_free(&frame);
    }

    AVFrame *frame = nullptr;
    int dmaFd = -1;

  ///  EGLImageKHR eglImage = EGL_NO_IMAGE_KHR;

 ///   GLuint texture = 0;
    VASurfaceID surface = VA_INVALID_SURFACE;

    AVBufferRef *device = nullptr;
     VADisplay display = nullptr;
};
#endif // VAAPIHARDWAREFRAME_H
