#include "videoframeconverter.h"

#include <QDebug>

#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include <QImage>

static void saveYuvFrame(const MediaFrame &frame)
{
    SwsContext *ctx =
        sws_getContext(frame.width,
                       frame.height,
                       AV_PIX_FMT_YUV420P,
                       frame.width,
                       frame.height,
                       AV_PIX_FMT_RGBA,
                       SWS_FAST_BILINEAR,
                       nullptr,
                       nullptr,
                       nullptr);

    if (!ctx)
        return;

    QImage img(frame.width,
               frame.height,
               QImage::Format_RGBA8888);

    uint8_t *dst[4];
    int dstStride[4];

    dst[0] = img.bits();
    dstStride[0] = img.bytesPerLine();

    sws_scale(ctx,
              frame.buffer->plane,
              frame.buffer->stride,
              0,
              frame.height,
              dst,
              dstStride);

    sws_freeContext(ctx);

    static int n = 0;

    if (n < 5)
    {
        img.save(QString("/tmp/converted_%1.png")
                 .arg(n));
    }

    n++;
}


VideoFrameConverter::VideoFrameConverter()
{

}



bool VideoFrameConverter::convertSwScale(
        const MediaFrame &input,
        MediaFrame &output,
        AVPixelFormat srcFormat)
{
    SwsContext *ctx =
        sws_getContext(
            input.width,
            input.height,
            srcFormat,
            input.width,
            input.height,
            AV_PIX_FMT_YUV420P,
            SWS_FAST_BILINEAR,
            nullptr,
            nullptr,
            nullptr);

    if (!ctx)
        return false;

    output.width  = input.width;
    output.height = input.height;
    output.format = PixelFormat::YUV420P;
    output.pts    = input.pts;
    output.sequence = input.sequence;

    output.buffer.reset(new MediaBuffer);

    MediaBuffer *buffer = output.buffer.data();

    int size =
        av_image_get_buffer_size(
            AV_PIX_FMT_YUV420P,
            output.width,
            output.height,
            1);

    buffer->storage.resize(size);

    av_image_fill_arrays(
        buffer->plane,
        buffer->stride,
        reinterpret_cast<uint8_t*>(buffer->storage.data()),
        AV_PIX_FMT_YUV420P,
        output.width,
        output.height,
        1);

    const uint8_t *srcData[4] = { nullptr };
    int srcStride[4] = { 0 };

    srcData[0] = input.buffer->plane[0];
    srcStride[0] = input.buffer->stride[0];

    sws_scale(ctx,
              srcData,
              srcStride,
              0,
              input.height,
              buffer->plane,
              buffer->stride);

    sws_freeContext(ctx);

    buffer->planeCount = 3;
//saveYuvFrame(output);
    return true;
}

bool VideoFrameConverter::convert(const MediaFrame &input,
                                  MediaFrame &output)
{
    switch (input.format)
    {
    case PixelFormat::RGBA8888:
        return convertSwScale(input, output,
                              AV_PIX_FMT_RGBA);

    case PixelFormat::RGB24:
        return convertSwScale(input, output,
                              AV_PIX_FMT_RGB24);

    case PixelFormat::YUYV:
        return convertSwScale(input, output,
                              AV_PIX_FMT_YUYV422);

    case PixelFormat::UYVY:
        return convertSwScale(input, output,
                              AV_PIX_FMT_UYVY422);

    case PixelFormat::NV12:
        return convertSwScale(input, output,
                              AV_PIX_FMT_NV12);

    case PixelFormat::YUV420P:
        output = input;
        return true;

    default:
        qWarning() << "Unsupported input format"
                   << static_cast<int>(input.format);
        return false;
    }
}

bool VideoFrameConverter::rgbaToYuv420(
        const MediaFrame &input,
        MediaFrame &output)
{

    SwsContext *ctx =
        sws_getContext(
            input.width,
            input.height,
            AV_PIX_FMT_RGBA,

            input.width,
            input.height,
            AV_PIX_FMT_YUV420P,

            SWS_FAST_BILINEAR,
            nullptr,
            nullptr,
            nullptr);



    if(!ctx)
        return false;



    output.width  = input.width;
    output.height = input.height;

    output.format =
            PixelFormat::YUV420P;



    output.buffer.reset(
            new MediaBuffer);



    MediaBuffer *buffer =
            output.buffer.data();



    int size =
        av_image_get_buffer_size(
            AV_PIX_FMT_YUV420P,
            output.width,
            output.height,
            1);



    buffer->storage.resize(size);



    av_image_fill_arrays(
        buffer->plane,
        buffer->stride,
        reinterpret_cast<uint8_t*>(
            buffer->storage.data()),

        AV_PIX_FMT_YUV420P,

        output.width,
        output.height,
        1);



    const uint8_t *srcData[1];

    int srcStride[1];


    srcData[0] =
        input.buffer->plane[0];


    srcStride[0] =
        input.buffer->stride[0];



    sws_scale(
        ctx,

        srcData,
        srcStride,

        0,
        input.height,

        buffer->plane,
        buffer->stride);



    sws_freeContext(ctx);



    buffer->planeCount = 3;


    return true;
}

