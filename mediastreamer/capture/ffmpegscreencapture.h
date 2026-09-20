#ifndef FFMPEGSCREENCAPTURE_H
#define FFMPEGSCREENCAPTURE_H

#include "../interfaces/IVideoCapture.h"

extern "C"
{
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
extern "C" {
#include <libavutil/imgutils.h>
}
extern "C" {
#include <libavutil/pixdesc.h>
}
extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}
class FFmpegScreenCapture : public IVideoCapture
{
public:
    FFmpegScreenCapture();
    ~FFmpegScreenCapture() override;

    bool open(const VideoCaptureSettings &settings) override;

    bool start() override;

    void stop() override;

    void close() override;

    bool grab(MediaFrame &frame) override;

    void release(MediaFrame &frame) override;

    void setMasterTime(MasterTime *masterTime) override;

private:

    bool openInput();

private:

    VideoCaptureSettings m_settings;

    MasterTime *m_masterTime = nullptr;

    bool m_running = false;

    AVFormatContext *m_format = nullptr;
    AVCodecContext *m_codec = nullptr;

    AVPacket *m_packet = nullptr;
    AVFrame *m_frame = nullptr;

    int m_streamIndex = -1;
};

#endif
