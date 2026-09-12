#include "videoconverter.h"

extern "C"
{
#include <libavutil/imgutils.h>
}

#include <QDebug>

VideoConverter::VideoConverter()
{
}

VideoConverter::~VideoConverter()
{
    if(m_context)
        sws_freeContext(m_context);
}

AVPixelFormat VideoConverter::pixelFormat(const MediaFrame &frame) const
{
    switch(frame.format)
    {
    case PixelFormat::NV12:
        return AV_PIX_FMT_NV12;

    case PixelFormat::YUV420P:
        return AV_PIX_FMT_YUV420P;

    case PixelFormat::YUYV:
        return AV_PIX_FMT_YUYV422;

    case PixelFormat::RGB24:
        return AV_PIX_FMT_RGB24;

    default:
        return AV_PIX_FMT_NONE;
    }
}

bool VideoConverter::createContext(const MediaFrame &frame)
{
    AVPixelFormat src = pixelFormat(frame);

    if(src == AV_PIX_FMT_NONE)
        return false;

    if(m_context &&
       m_width == frame.width &&
       m_height == frame.height &&
       m_sourceFormat == src)
    {
        return true;
    }

    if(m_context)
    {
        sws_freeContext(m_context);
        m_context = nullptr;
    }

    m_context = sws_getContext(
                frame.width,
                frame.height,
                src,

                frame.width,
                frame.height,
                AV_PIX_FMT_RGB24,

                SWS_BILINEAR,

                nullptr,
                nullptr,
                nullptr);

    if(!m_context)
    {
        qCritical() << "Cannot create SwsContext.";
        return false;
    }

    m_width = frame.width;
    m_height = frame.height;
    m_sourceFormat = src;

    return true;
}

bool VideoConverter::convert(const MediaFrame &frame,
                             QImage &image)
{
    if(frame.buffer.isNull())
        return false;

    if(!frame.buffer->isValid())
        return false;

    if(!createContext(frame))
        return false;

    if(image.isNull() ||
       image.width() != frame.width ||
       image.height() != frame.height)
    {
        image = QImage(frame.width,
                       frame.height,
                       QImage::Format_RGB888);
    }

    uint8_t *dstData[4] =
    {
        image.bits(),
        nullptr,
        nullptr,
        nullptr
    };

    int dstStride[4] =
    {
        image.bytesPerLine(),
        0,
        0,
        0
    };

    const uint8_t *srcData[4] =
    {
        frame.buffer->plane[0],
        frame.buffer->plane[1],
        frame.buffer->plane[2],
        frame.buffer->plane[3]
    };

    int srcStride[4] =
    {
        frame.buffer->stride[0],
        frame.buffer->stride[1],
        frame.buffer->stride[2],
        frame.buffer->stride[3]
    };

    int result = sws_scale(
                m_context,
                srcData,
                srcStride,
                0,
                frame.height,
                dstData,
                dstStride);

    if(result <= 0)
    {
        qWarning() << "sws_scale failed.";
        return false;
    }

    return true;
}
