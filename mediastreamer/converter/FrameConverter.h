#ifndef FRAMECONVERTER_H
#define FRAMECONVERTER_H

#include "../common/MediaFrame.h"

extern "C"
{
#include <libswscale/swscale.h>
}

class FrameConverter
{
public:

    FrameConverter();

    ~FrameConverter();

    bool convert(const MediaFrame &src,
                 PixelFormat dstFormat,
                 MediaFrame &dst);

private:

    bool createContext(const MediaFrame &src,
                       PixelFormat dstFormat);

    void allocateDestination(MediaFrame &dst,
                             PixelFormat format,
                             int width,
                             int height);

private:

    SwsContext *m_context=nullptr;

    int m_width=0;

    int m_height=0;

    PixelFormat m_srcFormat=PixelFormat::Unknown;

    PixelFormat m_dstFormat=PixelFormat::Unknown;
};

#endif
