#ifndef ABSTRACTVIDEORENDERER_H
#define ABSTRACTVIDEORENDERER_H

#include <QOpenGLWidget>
#include "../common/MediaFrame.h"

class AbstractVideoRenderer : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit AbstractVideoRenderer(QWidget *parent = nullptr)
        : QOpenGLWidget(parent)
    {
    }

    virtual ~AbstractVideoRenderer() = default;

    virtual void onFrame(const MediaFrame &frame) = 0;
};

#endif
