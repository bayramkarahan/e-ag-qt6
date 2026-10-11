#include "systemaudiocapture.h"

#include <QDebug>

#include <pulse/error.h>
#include <pulse/sample.h>

namespace {

struct MonitorLookup
{
    QByteArray sinkName;
    QByteArray monitorName;
    bool done = false;
};

void sinkInfoCallback(pa_context *context,
                      const pa_sink_info *info,
                      int eol,
                      void *userdata)
{
    Q_UNUSED(context);

    auto *lookup = static_cast<MonitorLookup *>(userdata);

    if (eol < 0) {
        lookup->done = true;
        return;
    }

    if (info && info->monitor_source_name) {
        lookup->monitorName = info->monitor_source_name;
    }

    if (eol > 0) {
        lookup->done = true;
    }
}

void serverInfoCallback(pa_context *context,
                        const pa_server_info *info,
                        void *userdata)
{
    auto *lookup = static_cast<MonitorLookup *>(userdata);

    if (!info || !info->default_sink_name) {
        lookup->done = true;
        return;
    }

    lookup->sinkName = info->default_sink_name;

    pa_operation *operation =
        pa_context_get_sink_info_by_name(
            context,
            lookup->sinkName.constData(),
            sinkInfoCallback,
            lookup);

    if (operation) {
        pa_operation_unref(operation);
    } else {
        lookup->done = true;
    }
}

QByteArray findMonitorSource()
{
    pa_mainloop *mainloop = pa_mainloop_new();

    if (!mainloop) {
        qWarning() << "Cannot create PulseAudio mainloop.";
        return {};
    }

    pa_mainloop_api *api = pa_mainloop_get_api(mainloop);

    pa_context *context =
        pa_context_new(api, "MediaStreamer-MonitorLookup");

    if (!context) {
        qWarning() << "Cannot create PulseAudio context.";
        pa_mainloop_free(mainloop);
        return {};
    }

    if (pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr)
        < 0) {
        qWarning() << "Cannot connect to audio server:"
                   << pa_strerror(pa_context_errno(context));

        pa_context_unref(context);
        pa_mainloop_free(mainloop);
        return {};
    }

    while (pa_context_get_state(context) != PA_CONTEXT_READY) {
        const pa_context_state_t state =
            pa_context_get_state(context);

        if (!PA_CONTEXT_IS_GOOD(state)) {
            qWarning() << "Audio server connection failed:"
                       << pa_strerror(pa_context_errno(context));

            pa_context_disconnect(context);
            pa_context_unref(context);
            pa_mainloop_free(mainloop);
            return {};
        }

        if (pa_mainloop_iterate(mainloop, 1, nullptr) < 0) {
            qWarning() << "Audio server mainloop failed.";

            pa_context_disconnect(context);
            pa_context_unref(context);
            pa_mainloop_free(mainloop);
            return {};
        }
    }

    MonitorLookup lookup;

    pa_operation *operation =
        pa_context_get_server_info(
            context,
            serverInfoCallback,
            &lookup);

    if (operation) {
        pa_operation_unref(operation);
    }

    while (!lookup.done) {
        if (pa_mainloop_iterate(mainloop, 1, nullptr) < 0) {
            qWarning() << "Monitor lookup failed.";
            break;
        }
    }

    pa_context_disconnect(context);
    pa_context_unref(context);
    pa_mainloop_free(mainloop);

    if (lookup.monitorName.isEmpty()) {
        qWarning() << "No monitor source found for default output."
                   << "Default sink:" << lookup.sinkName;
    } else {
        qDebug() << "Default audio sink:" << lookup.sinkName;
        qDebug() << "System audio monitor:" << lookup.monitorName;
    }

    return lookup.monitorName;
}

} // namespace


SystemAudioCapture::SystemAudioCapture(QObject *parent)
    : QObject(parent)
{
}

SystemAudioCapture::~SystemAudioCapture()
{
    close();
}

void SystemAudioCapture::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
}

bool SystemAudioCapture::open(int sampleRate, int channels)
{
    close();

    if (sampleRate <= 0 || channels <= 0) {
        qWarning() << "Invalid system audio format:"
                   << sampleRate << channels;
        return false;
    }

    const QByteArray monitorName = findMonitorSource();

    if (monitorName.isEmpty()) {
        qWarning() << "System audio capture could not start.";
        return false;
    }

    m_sampleRate = sampleRate;
    m_channels = channels;
    m_buffer.clear();
    m_pts = 0;

    pa_sample_spec spec;
    spec.format = PA_SAMPLE_S16LE;
    spec.rate = static_cast<uint32_t>(m_sampleRate);
    spec.channels = static_cast<uint8_t>(m_channels);

    if (!pa_sample_spec_valid(&spec)) {
        qWarning() << "Unsupported PulseAudio sample format.";
        return false;
    }

    int error = 0;

    m_stream = pa_simple_new(
        nullptr,
        "MediaStreamer",
        PA_STREAM_RECORD,
        monitorName.constData(),
        "System Audio Capture",
        &spec,
        nullptr,
        nullptr,
        &error);

    if (!m_stream) {
        qWarning() << "Cannot open system audio monitor:"
                   << pa_strerror(error);
        return false;
    }

    m_running.store(true);

    m_thread = std::thread([this]() {
        captureLoop();
    });

    qDebug() << "System audio capture started."
             << "Source:" << monitorName
             << "Rate:" << m_sampleRate
             << "Channels:" << m_channels;

    return true;
}

void SystemAudioCapture::close()
{
    m_running.store(false);

    if (m_thread.joinable()) {
        m_thread.join();
    }

    if (m_stream) {
        pa_simple_free(m_stream);
        m_stream = nullptr;
    }

    m_buffer.clear();
    m_pts = 0;
}

void SystemAudioCapture::captureLoop()
{
    constexpr int BytesPerSample = 2;
    constexpr int FramesPerSecond = 50; // 20 ms

    const int samplesPerFrame = m_sampleRate / FramesPerSecond;
    const int frameBytes =
        samplesPerFrame * m_channels * BytesPerSample;

    if (frameBytes <= 0) {
        qWarning() << "Invalid system audio frame size.";
        return;
    }

    QByteArray data(frameBytes, '\0');

    while (m_running.load()) {
        int error = 0;

        if (pa_simple_read(
                m_stream,
                data.data(),
                static_cast<size_t>(frameBytes),
                &error) < 0) {
            if (m_running.load()) {
                qWarning() << "System audio read failed:"
                           << pa_strerror(error);
            }
            break;
        }

        if (!m_running.load()) {
            break;
        }

        AudioFrame frame;
        frame.data = data;
        frame.sampleRate = m_sampleRate;
        frame.channels = m_channels;
        frame.sampleSize = 16;

        if (m_masterTime) {
            frame.pts = m_masterTime->nowUs();
        } else {
            frame.pts = m_pts;
        }

        m_pts +=
            (static_cast<qint64>(samplesPerFrame) * 1000000)
            / m_sampleRate;

        emit frameReady(frame);
    }
}
