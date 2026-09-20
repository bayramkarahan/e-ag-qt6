#include "testvideowidget.h"

#include <QPainter>

TestVideoWidget::TestVideoWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(640,480);
}

void TestVideoWidget::setImage(const QImage &image)
{
    QMutexLocker locker(&m_mutex);

    m_image=image.copy();

    update();
}

void TestVideoWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(rect(),Qt::black);

    QMutexLocker locker(&m_mutex);

    if(m_image.isNull())
        return;

    painter.drawImage(rect(),m_image);
}
