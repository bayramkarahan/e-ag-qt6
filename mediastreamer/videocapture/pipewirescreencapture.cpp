#include "pipewirescreencapture.h"
#include <QMutexLocker>
#include <cstring>
extern "C" {
#include <spa/param/format.h>
#include <spa/param/video/raw.h>
#include <spa/param/video/format-utils.h>
#include <spa/buffer/buffer.h>
#include <spa/pod/builder.h>
}
#include <QMutexLocker>
#include <cstring>
#include <algorithm>
#include <spa/debug/types.h>
#include <QElapsedTimer>
static const pw_stream_events streamEvents = [] {
    pw_stream_events events{};
    events.version = PW_VERSION_STREAM_EVENTS;
    events.state_changed = PipeWireScreenCapture::onStateChanged;
    events.param_changed = PipeWireScreenCapture::onParamChanged;
    events.process = PipeWireScreenCapture::onProcess;
    return events;
}();

PipeWireScreenCapture::PipeWireScreenCapture()
{

}
void PipeWireScreenCapture::onStateChanged(
    void *data,
    pw_stream_state oldState,
    pw_stream_state state,
    const char *error)
{
    Q_UNUSED(data);
    Q_UNUSED(oldState);

    qDebug() << "PipeWire stream state:"
             << pw_stream_state_as_string(state);

    if (error) {
        qWarning() << "PipeWire stream error:" << error;
    }
}
void PipeWireScreenCapture::onParamChanged(
    void *data,
    uint32_t id,
    const spa_pod *param)
{
    Q_UNUSED(id);

    auto *self = static_cast<PipeWireScreenCapture *>(data);

    if (!param)
        return;

    spa_video_info_raw info{};

    if (spa_format_video_raw_parse(param, &info) < 0)
        return;

    self->m_width = info.size.width;
    self->m_height = info.size.height;
if (info.format == SPA_VIDEO_FORMAT_BGRA)
    self->m_format = PixelFormat::BGRA8888;
else if (info.format == SPA_VIDEO_FORMAT_RGBA)
    self->m_format = PixelFormat::RGBA8888;
else
    qWarning() << "Desteklenmeyen PipeWire piksel formatı:"
               << info.format;
               
qDebug() << "PipeWire SPA format:"
         << spa_debug_type_find_name(
                spa_type_video_format,
                info.format);
                
    qDebug() << "PipeWire video format:"
             << self->m_width
             << "x" << self->m_height
             << "format =" << info.format;
}
void PipeWireScreenCapture::onProcess(void *userdata)
{
    auto *self = static_cast<PipeWireScreenCapture *>(userdata);

    if (!self || !self->m_stream)
        return;

    pw_buffer *buffer = pw_stream_dequeue_buffer(self->m_stream);
    if (!buffer)
        return;

    spa_buffer *spaBuffer = buffer->buffer;

    if (spaBuffer && spaBuffer->n_datas > 0 &&
        self->m_width > 0 && self->m_height > 0) {

        spa_data *data = &spaBuffer->datas[0];
        spa_chunk *chunk = data->chunk;

        if (data->data && chunk && chunk->size > 0) {
            const int width = self->m_width;
            const int height = self->m_height;
            const int rowBytes = width * 4;
            const int sourceStride = chunk->stride > 0
                                         ? chunk->stride
                                         : rowBytes;

            const quint64 offset = chunk->offset;
            const quint64 size = chunk->size;
            const quint64 maxSize = data->maxsize;

            const bool valid =
                sourceStride >= rowBytes &&
                offset <= maxSize &&
                size <= maxSize - offset &&
                size >= quint64(sourceStride) * quint64(height - 1) +
                            quint64(rowBytes);

            if (valid) {
                auto frameBuffer = QSharedPointer<MediaBuffer>::create();
                frameBuffer->storage.resize(rowBytes * height);

                const auto *src =
                    static_cast<const uchar *>(data->data) + offset;
                auto *dst =
                    reinterpret_cast<uchar *>(
                        frameBuffer->storage.data());

                for (int y = 0; y < height; ++y) {
                    std::memcpy(dst + y * rowBytes,
                                src + y * sourceStride,
                                rowBytes);
                }

                frameBuffer->plane[0] = dst;
                frameBuffer->stride[0] = rowBytes;
                frameBuffer->planeSize[0] = rowBytes * height;
                frameBuffer->planeCount = 1;

                MediaFrame frame;
                frame.buffer = frameBuffer;
                frame.width = width;
                frame.height = height;
                frame.format = self->m_format;
                frame.pts = self->m_pts++;

                {
                    QMutexLocker locker(&self->m_frameMutex);
                    self->m_latestFrame = frame;
                    self->m_hasFrame = true;
                    static int frameCount = 0;
static QElapsedTimer frameTimer;

if (frameCount == 0)
    frameTimer.start();

++frameCount;

if (frameCount <= 10 || frameCount % 60 == 0) {
    qDebug() << "PipeWire frame:"
             << "count =" << frameCount
             << "elapsed(ms) =" << frameTimer.elapsed()
             << "size =" << chunk->size
             << "stride =" << chunk->stride;
}


                }
                
                
            }
        }
    }



    pw_stream_queue_buffer(self->m_stream, buffer);
}
bool PipeWireScreenCapture::setPortalConnection(
    int fd,
    uint nodeId)
{
    if (fd < 0 || nodeId == PW_ID_ANY) {
        qWarning() << "Invalid portal FD or node ID";
        return false;
    }

    m_portalFd = fd;
    m_nodeId = nodeId;

    qDebug() << "Portal connection configured:"
             << "FD =" << m_portalFd
             << "Node =" << m_nodeId;

    return true;
}

bool PipeWireScreenCapture::open(const VideoCaptureSettings &settings)
{
    m_settings = settings;

    pw_init(nullptr, nullptr);

    m_loop = pw_main_loop_new(nullptr);

    if(!m_loop)
    {
        qWarning() << "pw_main_loop_new failed";
        return false;
    }

    m_context =
            pw_context_new(
                pw_main_loop_get_loop(m_loop),
                nullptr,
                0);

    if(!m_context)
    {
        qWarning() << "pw_context_new failed";
        close();
        return false;
    }

if (m_portalFd < 0 || m_nodeId == PW_ID_ANY) {
    qWarning() << "Portal connection information is missing";
    close();
    return false;
}

m_core = pw_context_connect_fd(
    m_context,
    m_portalFd,
    nullptr,
    0);

if (!m_core) {
    qWarning() << "pw_context_connect_fd failed";
    close();
    return false;
}

// FD artık PipeWire bağlantısına devredildi.
m_portalFd = -1;


    if(!m_core)
    {
        qWarning() << "pw_context_connect failed";
        close();
        return false;
    }

    pw_properties *props =
            pw_properties_new(
                PW_KEY_MEDIA_TYPE, "Video",
                PW_KEY_MEDIA_CATEGORY, "Capture",
                PW_KEY_MEDIA_ROLE, "Screen",
                nullptr);

    m_stream =
            pw_stream_new(
                m_core,
                "MediaStreamer Screen Capture",
                props);

    if(!m_stream)
    {
        qWarning() << "pw_stream_new failed";
        close();
        return false;
    }

    qDebug() << "PipeWire initialized.";
pw_stream_add_listener(
    m_stream,
    &m_streamListener,
    &streamEvents,
    this
);

qDebug() << "PipeWire stream listener eklendi.";
return true;

}

void PipeWireScreenCapture::close()
{
    stop();

    if(m_stream)
    {
        pw_stream_destroy(m_stream);
        m_stream = nullptr;
    }

    if(m_core)
    {
        pw_core_disconnect(m_core);
        m_core = nullptr;
    }

    if(m_context)
    {
        pw_context_destroy(m_context);
        m_context = nullptr;
    }

    if(m_loop)
    {
        pw_main_loop_destroy(m_loop);
        m_loop = nullptr;
    }
}
bool PipeWireScreenCapture::start()
{
    if (!m_stream || !m_loop || m_nodeId == PW_ID_ANY) {
        qWarning() << "PipeWire stream hazır değil.";
        return false;
    }

    uint8_t buffer[1024];

    spa_pod_builder builder =
        SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

    const spa_pod *params[1];

    params[0] = static_cast<const spa_pod *>(
        spa_pod_builder_add_object(
            &builder,
            SPA_TYPE_OBJECT_Format,
            SPA_PARAM_EnumFormat,
            SPA_FORMAT_mediaType,
            SPA_POD_Id(SPA_MEDIA_TYPE_video),
            SPA_FORMAT_mediaSubtype,
            SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
            SPA_FORMAT_VIDEO_format, SPA_POD_Id(SPA_VIDEO_FORMAT_BGRA)
        )
    );

    const int result = pw_stream_connect(
        m_stream,
        PW_DIRECTION_INPUT,
        m_nodeId,
        static_cast<pw_stream_flags>(
            PW_STREAM_FLAG_AUTOCONNECT |
            PW_STREAM_FLAG_MAP_BUFFERS
        ),
        params,
        1
    );

    if (result < 0) {
        qWarning() << "pw_stream_connect başarısız:" << result;
        return false;
    }

    m_running = true;
    qDebug() << "PipeWire stream bağlantısı başlatıldı.";
    return true;
}

void PipeWireScreenCapture::stop()
{
    m_running = false;
}

void PipeWireScreenCapture::release(MediaFrame &frame)
{
    Q_UNUSED(frame);
}

void PipeWireScreenCapture::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
}

bool PipeWireScreenCapture::grab(MediaFrame &m_frame)
{
    if (!m_running || !m_loop)
        return false;

    // PipeWire callback'lerini mevcut yakalama iş parçacığında işle.
    pw_loop_iterate(pw_main_loop_get_loop(m_loop), 0);

    QMutexLocker locker(&m_frameMutex);

    if (!m_hasFrame)
        return false;

    m_frame = m_latestFrame;
    m_hasFrame = false;

    return true;
}
PipeWireScreenCapture::~PipeWireScreenCapture()
{
    close();
}
