#include "FrameConverter.h"

#include <QDebug>

extern "C"
{
#include <libavutil/imgutils.h>
}
namespace
{

AVPixelFormat toFFmpegFormat(PixelFormat format)
{
    switch(format)
    {
    case PixelFormat::YUYV:
        return AV_PIX_FMT_YUYV422;

    case PixelFormat::NV12:
        return AV_PIX_FMT_NV12;

    case PixelFormat::YUV420P:
        return AV_PIX_FMT_YUV420P;

    case PixelFormat::RGB24:
        return AV_PIX_FMT_RGB24;

    default:
        return AV_PIX_FMT_NONE;
    }
}

}
FrameConverter::FrameConverter()
{
}

FrameConverter::~FrameConverter()
{
    if(m_context)
    {
        sws_freeContext(m_context);
        m_context = nullptr;
    }
}
bool FrameConverter::createContext(const MediaFrame &src,
                                   PixelFormat dstFormat)
{
    if(m_context &&
       m_width == src.width &&
       m_height == src.height &&
       m_srcFormat == src.format &&
       m_dstFormat == dstFormat)
    {
        return true;
    }

    if(m_context)
    {
        sws_freeContext(m_context);
        m_context = nullptr;
    }

    AVPixelFormat srcFmt = toFFmpegFormat(src.format);
    AVPixelFormat dstFmt = toFFmpegFormat(dstFormat);

    if(srcFmt == AV_PIX_FMT_NONE ||
       dstFmt == AV_PIX_FMT_NONE)
    {
        qWarning() << "Unsupported pixel format.";
        return false;
    }

    m_context = sws_getContext(
                src.width,
                src.height,
                srcFmt,

                src.width,
                src.height,
                dstFmt,

                SWS_FAST_BILINEAR,

                nullptr,
                nullptr,
                nullptr);

    if(!m_context)
    {
        qCritical() << "Cannot create SwsContext.";
        return false;
    }

    m_width = src.width;
    m_height = src.height;

    m_srcFormat = src.format;
    m_dstFormat = dstFormat;

    return true;
}

void FrameConverter::allocateDestination(MediaFrame &dst,
                                         PixelFormat format,
                                         int width,
                                         int height)
{
    dst.width  = width;
    dst.height = height;
    dst.format = format;

    dst.buffer = QSharedPointer<MediaBuffer>::create();

    MediaBuffer *buffer = dst.buffer.data();

    AVPixelFormat ffFormat = toFFmpegFormat(format);

    int totalSize =
            av_image_get_buffer_size(ffFormat,
                                     width,
                                     height,
                                     1);

    if(totalSize <= 0)
    {
        qWarning() << "Invalid image size.";
        return;
    }

    buffer->storage.resize(totalSize);

    uchar *base =
            reinterpret_cast<uchar*>(buffer->storage.data());

    av_image_fill_arrays(
                buffer->plane,
                buffer->stride,
                base,
                ffFormat,
                width,
                height,
                1);

    switch(format)
    {
    case PixelFormat::RGB24:

        buffer->planeCount = 1;

        buffer->planeSize[0] = totalSize;

        break;

    case PixelFormat::YUV420P:

        buffer->planeCount = 3;

        buffer->planeSize[0] =
                buffer->stride[0] * height;

        buffer->planeSize[1] =
                buffer->stride[1] * (height / 2);

        buffer->planeSize[2] =
                buffer->stride[2] * (height / 2);

        break;

    case PixelFormat::NV12:

        buffer->planeCount = 2;

        buffer->planeSize[0] =
                buffer->stride[0] * height;

        buffer->planeSize[1] =
                buffer->stride[1] * (height / 2);

        break;

    default:

        break;
    }
}

bool FrameConverter::convert(const MediaFrame &src,
                             PixelFormat dstFormat,
                             MediaFrame &dst)
{
    if(!src.buffer)
        return false;

    if(!createContext(src, dstFormat))
        return false;

    if(!dst.buffer ||
       dst.width  != src.width ||
       dst.height != src.height ||
       dst.format != dstFormat)
    {
        allocateDestination(dst,
                            dstFormat,
                            src.width,
                            src.height);
    }

    const uint8_t *srcData[4] =
    {
        src.buffer->plane[0],
        src.buffer->plane[1],
        src.buffer->plane[2],
        src.buffer->plane[3]
    };

    int srcStride[4] =
    {
        src.buffer->stride[0],
        src.buffer->stride[1],
        src.buffer->stride[2],
        src.buffer->stride[3]
    };

    uint8_t *dstData[4] =
    {
        dst.buffer->plane[0],
        dst.buffer->plane[1],
        dst.buffer->plane[2],
        dst.buffer->plane[3]
    };

    int dstStride[4] =
    {
        dst.buffer->stride[0],
        dst.buffer->stride[1],
        dst.buffer->stride[2],
        dst.buffer->stride[3]
    };

    int ret = sws_scale(m_context,
                        srcData,
                        srcStride,
                        0,
                        src.height,
                        dstData,
                        dstStride);

    if(ret <= 0)
    {
        qWarning() << "sws_scale failed.";
        return false;
    }

    dst.width  = src.width;
    dst.height = src.height;

    dst.pts = src.pts;
    dst.keyFrame = src.keyFrame;

    return true;
}

