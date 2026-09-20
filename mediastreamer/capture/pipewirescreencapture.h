#ifndef PIPEWIRESCREENCAPTURE_H
#define PIPEWIRESCREENCAPTURE_H

#include "../interfaces/IVideoCapture.h"
#include<QDebug>

extern "C"
{
#include <pipewire/pipewire.h>
#include <spa/param/video/format-utils.h>
}

class PipeWireScreenCapture : public IVideoCapture
{
public:
    PipeWireScreenCapture();
    ~PipeWireScreenCapture() override;

    bool open(const VideoCaptureSettings &settings) override;

    bool start() override;

    void stop() override;

    void close() override;

    bool grab(MediaFrame &m_frame) override;

    void release(MediaFrame &frame) override;

    void setMasterTime(MasterTime *masterTime) override;

private:

    static void onProcess(void *userdata);

    bool dequeueFrame();

private:

    VideoCaptureSettings m_settings;

    MasterTime *m_masterTime = nullptr;

    bool m_running = false;

    pw_main_loop *m_loop = nullptr;

    pw_context *m_context = nullptr;

    pw_core *m_core = nullptr;

    pw_stream *m_stream = nullptr;

    spa_hook m_streamListener {};

    QSharedPointer<MediaBuffer> m_buffer;

    int m_width = 0;

    int m_height = 0;

    PixelFormat m_format = PixelFormat::RGBA8888;

    qint64 m_pts = 0;
};

#endif
