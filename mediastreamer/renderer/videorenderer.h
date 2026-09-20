#ifndef VIDEORENDERER_H
#define VIDEORENDERER_H

#include <QWidget>
#include <QImage>
#include <QMutex>

class VideoRenderer : public QWidget
{
    Q_OBJECT

public:

    explicit VideoRenderer(QWidget *parent = nullptr);

    void setImage(const QImage &image);

    void clear();

protected:

    void paintEvent(QPaintEvent *event) override;

private:

    mutable QMutex m_mutex;

    QImage m_image;
};

#endif
