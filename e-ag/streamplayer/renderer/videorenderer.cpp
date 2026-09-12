#include "videorenderer.h"

#include <QPainter>
#include <QMutexLocker>

VideoRenderer::VideoRenderer(QWidget *parent)
    : AbstractVideoRenderer(parent)
{
    setMinimumSize(640,480);

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAutoFillBackground(false);
}

void VideoRenderer::onFrame(const MediaFrame &frame)
{


     QImage image;

     if(!m_converter.convert(frame, image))
         return;

     setImage(image);
}
void VideoRenderer::setImage(const QImage &image)
{
   /* static int frameNo = 0;

    if (frameNo < 5)
    {
        image.save(QString("/tmp/render_%1.png")
                       .arg(frameNo, 3, 10, QLatin1Char('0')));

        qDebug() << "Saved renderer image";
    }

    frameNo++;
*/
    QMutexLocker locker(&m_mutex);

    m_image = image;

    update();
}


void VideoRenderer::clear()
{
    QMutexLocker locker(&m_mutex);

    m_image = QImage();

    update();
}

void VideoRenderer::paintEvent(QPaintEvent *)
{
    ///qDebug() << "VIDEO PAINT";
    QPainter painter(this);

    painter.fillRect(rect(), Qt::black);

    QMutexLocker locker(&m_mutex);

    if(m_image.isNull())
        return;

    QSize imageSize = m_image.size();
    imageSize.scale(size(), Qt::KeepAspectRatio);

    QRect target(
                QPoint((width() - imageSize.width()) / 2,
                       (height() - imageSize.height()) / 2),
                imageSize);

    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter.drawImage(target, m_image);
}
