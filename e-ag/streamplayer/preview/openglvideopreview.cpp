#include "openglvideopreview.h"

OpenGLVideoPreview::OpenGLVideoPreview(OpenGLVideoRenderer *renderer,
                                       QObject *parent)
    : QObject(parent),
      m_renderer(renderer)
{
}

void OpenGLVideoPreview::setMediaClock(MediaClock *clock)
{
    m_clock = clock;
}

void OpenGLVideoPreview::onFrame(const MediaFrame &frame)
{
    if (!m_renderer)
        return;

    /*
    if (m_clock)
    {
        qDebug()
                << "Video latency ="
                << (m_clock->nowUs() - frame.pts) / 1000.0
                << "ms";
    }
    */

    m_renderer->onFrame(frame);
}
