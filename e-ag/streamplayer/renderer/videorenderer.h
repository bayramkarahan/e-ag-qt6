#ifndef VIDEORENDERER_H
#define VIDEORENDERER_H

#include <QWidget>
#include <QImage>
#include <QMutex>
#include "AbstractVideoRenderer.h"

#include "../converter/videoconverter.h"
class VideoRenderer :public AbstractVideoRenderer
{
    Q_OBJECT

public:

    explicit VideoRenderer(QWidget *parent = nullptr);

    void setImage(const QImage &image);

    void onFrame(const MediaFrame &frame);
    void clear();

protected:

    void paintEvent(QPaintEvent *event) override;

private:

    mutable QMutex m_mutex;
    VideoConverter m_converter;
    QImage m_image;
};

#endif
