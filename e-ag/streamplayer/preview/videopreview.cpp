#include "videopreview.h"

#include <QPainter>
#include <QMutexLocker>

VideoPreview::VideoPreview(QWidget *parent)
    : QWidget(parent)
{
    ///setMinimumSize(640,480);

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAutoFillBackground(false);
}

void VideoPreview::setSettings(const StreamSettings &settings)
{
    m_settings=settings;
}



void VideoPreview::onFrame(const MediaFrame &frame)
{
    // 200 ms dolmadıysa bu kareyi işleme.
    if(m_settings.previewMode==PreviewMode::MultiPreview)
    {
        if (m_frameLimiter.isValid() &&
            m_frameLimiter.elapsed() < FRAME_INTERVAL_MS) {
            return;
        }
            //qDebug() << "Saved renderer image"<<int(m_settings.previewMode);
        m_frameLimiter.start();
    }

    QImage image;

    if (!m_converter.convert(frame, image))
        return;

    setImage(image);
}

void VideoPreview::setImage(const QImage &image)
{
   QMutexLocker locker(&m_mutex);
    m_image = image;
    update();
}


void VideoPreview::clear()
{
    QMutexLocker locker(&m_mutex);
    m_image = QImage();
    update();
}



void VideoPreview::paintEvent(QPaintEvent *)
{
    QImage image;

    {
        QMutexLocker locker(&m_mutex);
        image = m_image;
    }

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (image.isNull())
        return;

    // Görüntü zaten aynı boyuttaysa ölçekleme yapma.
    if (image.size() == size()) {
        painter.drawImage(QPoint(0, 0), image);
        return;
    }

    // Görüntünün tamamını, en-boy oranını koruyarak sığdır.
    QSize targetSize = image.size();
    targetSize.scale(size(), Qt::KeepAspectRatio);

    const QRect targetRect(
        (width() - targetSize.width()) / 2,
        (height() - targetSize.height()) / 2,
        targetSize.width()+4,
        targetSize.height()
    );

    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter.drawImage(targetRect, image);
}

/*
void VideoPreview::paintEvent(QPaintEvent *)
{
    QImage image;

    {
        QMutexLocker locker(&m_mutex);
        image = m_image;
    }

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (image.isNull())
        return;

    QSize targetSize = image.size();
    targetSize.scale(size(), Qt::KeepAspectRatio);

    QRect targetRect(
        (width() - targetSize.width()) / 2,
        (height() - targetSize.height()) / 2,
        targetSize.width(),
        targetSize.height()
    );

    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter.drawImage(targetRect, image);
}




void VideoPreview::paintEvent(QPaintEvent *)
{
    QImage image;
    {
        QMutexLocker locker(&m_mutex);
        image = m_image;
    }

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (image.isNull() || width() <= 0 || height() <= 0)
        return;

    const int iw = image.width();
    const int ih = image.height();
    const int ww = width();
    const int wh = height();

    QRect source(0, 0, iw, ih);

    if (static_cast<qint64>(iw) * wh >
        static_cast<qint64>(ih) * ww) {
        const int cropWidth = static_cast<int>(
            static_cast<qint64>(ih) * ww / wh);
        source.setLeft((iw - cropWidth) / 2);
        source.setWidth(cropWidth);
    } else {
        const int cropHeight = static_cast<int>(
            static_cast<qint64>(iw) * wh / ww);
        source.setTop((ih - cropHeight) / 2);
        source.setHeight(cropHeight);
    }

    painter.drawImage(rect(), image, source);
}



/*
void VideoPreview::paintEvent(QPaintEvent *)
{
    QImage image;

    {
        QMutexLocker locker(&m_mutex);
        image = m_image;
    }

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (image.isNull())
        return;

    QSize targetSize = image.size();
    targetSize.scale(size(), Qt::KeepAspectRatio);

    const QRect targetRect(
        (width() - targetSize.width()) / 2,
        (height() - targetSize.height()) / 2,
        targetSize.width(),
        targetSize.height()
    );

    painter.setRenderHint(
        QPainter::SmoothPixmapTransform, false);

    painter.drawImage(targetRect, image);
}
*/
