#ifndef PIPEWIRESCREENCAPTURE_H
#define PIPEWIRESCREENCAPTURE_H
#include <QMutex>
#include "../common/MediaFrame.h"
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
    bool setPortalConnection(int fd, uint nodeId);
static void onStateChanged(
    void *data,
    pw_stream_state oldState,
    pw_stream_state state,
    const char *error);

static void onParamChanged(
    void *data,
    uint32_t id,
    const spa_pod *param);
QByteArray m_frameData;

    static void onProcess(void *userdata);

private:

    bool dequeueFrame();

private:
QMutex m_frameMutex;
MediaFrame m_latestFrame;
bool m_hasFrame = false;

int m_portalFd = -1;
uint32_t m_nodeId = PW_ID_ANY;

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
