#include "videopreview.h"

VideoPreview::VideoPreview(VideoRenderer *renderer,
                           QObject *parent)
    : QObject(parent),
      m_renderer(renderer)
{
}

void VideoPreview::onFrame(const MediaFrame &frame)
{
   //qDebug() << "PREVIEW";
    if(m_renderer == nullptr)
        return;
    /*qDebug()<<"VideoPreview::onFrame"
        << frame.width
        << frame.height
        << int(frame.format)
        << frame.buffer->stride[0]
        << frame.buffer->plane[0];*/

    QImage image;

    if(!m_converter.convert(frame, image))
        return;
  /*  if(m_masterTime)
    {
        qDebug()
            << "Video latency ="
            << (m_masterTime->nowUs() - frame.pts) / 1000.0
            << "ms";
    }*/
    m_renderer->setImage(image);
}

void VideoPreview::setMediaClock(MediaClock *masterTime)
{
    m_clock = masterTime;
}
