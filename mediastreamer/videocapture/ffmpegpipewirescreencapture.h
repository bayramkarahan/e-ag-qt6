#ifndef FFMPEGPIPEWIRESCREENCAPTURE_H
#define FFMPEGPIPEWIRESCREENCAPTURE_H

#include "../interfaces/IVideoCapture.h"
#include<QDebug>
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

class FFmpegPipeWireScreenCapture : public IVideoCapture
{
public:
    FFmpegPipeWireScreenCapture();
    ~FFmpegPipeWireScreenCapture() override;

    bool open(const VideoCaptureSettings &settings) override;

    bool start() override;

    void stop() override;

    void close() override;

    bool grab(MediaFrame &frame) override;

    void release(MediaFrame &frame) override;

    void setMasterTime(MasterTime *masterTime) override;

private:
    AVFormatContext *m_format = nullptr;
    AVCodecContext  *m_codec  = nullptr;

    AVPacket *m_packet = nullptr;
    AVFrame  *m_frame  = nullptr;

    int m_streamIndex = -1;

    bool m_running = false;

    VideoCaptureSettings m_settings;

    MasterTime *m_masterTime = nullptr;
};

#endif
