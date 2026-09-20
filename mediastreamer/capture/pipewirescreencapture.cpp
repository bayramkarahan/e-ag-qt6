#include "pipewirescreencapture.h"

PipeWireScreenCapture::PipeWireScreenCapture()
{

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

    m_core =
            pw_context_connect(
                m_context,
                nullptr,
                0);

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
    m_running = true;
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
    qDebug()
        << m_frame.width
        << m_frame.height
        << int(m_frame.format);
}

PipeWireScreenCapture::~PipeWireScreenCapture()
{
    close();
}
