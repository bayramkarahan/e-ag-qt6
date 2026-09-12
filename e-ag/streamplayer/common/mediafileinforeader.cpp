#include "mediafileinforeader.h"

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

MediaFileInfo MediaFileInfoReader::read(const QString &fileName)
{
    MediaFileInfo info;

    AVFormatContext *fmt = nullptr;

    if (avformat_open_input(&fmt,
                            fileName.toUtf8().constData(),
                            nullptr,
                            nullptr) < 0)
    {
        return info;
    }

    if (avformat_find_stream_info(fmt, nullptr) < 0)
    {
        avformat_close_input(&fmt);
        return info;
    }

    info.valid = true;

    for (unsigned int i = 0; i < fmt->nb_streams; ++i)
    {
        AVStream *stream = fmt->streams[i];
        AVCodecParameters *par = stream->codecpar;

        if (par->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            info.hasVideo = true;

            info.width  = par->width;
            info.height = par->height;

            if (stream->avg_frame_rate.den != 0)
                info.fps = av_q2d(stream->avg_frame_rate);

            info.videoCodec = avcodec_get_name(par->codec_id);
        }
        else if (par->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            info.hasAudio = true;

            info.sampleRate = par->sample_rate;

#if LIBAVCODEC_VERSION_MAJOR >= 59
            info.channels = par->ch_layout.nb_channels;
#else
            info.channels = par->channels;
#endif

            info.audioCodec = avcodec_get_name(par->codec_id);
        }
    }

    avformat_close_input(&fmt);

    return info;
}
