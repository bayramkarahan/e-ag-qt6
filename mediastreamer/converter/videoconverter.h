#ifndef VIDEOCONVERTER_H
#define VIDEOCONVERTER_H

#include <QImage>

extern "C"
{
#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
}

#include "../common/MediaFrame.h"

class VideoConverter
{
public:

    VideoConverter();

    ~VideoConverter();

    bool convert(const MediaFrame &frame,
                 QImage &image);

private:

    bool createContext(const MediaFrame &frame);

    AVPixelFormat pixelFormat(const MediaFrame &frame) const;

private:

    SwsContext *m_context = nullptr;

    int m_width = 0;

    int m_height = 0;

    AVPixelFormat m_sourceFormat = AV_PIX_FMT_NONE;
};

#endif
