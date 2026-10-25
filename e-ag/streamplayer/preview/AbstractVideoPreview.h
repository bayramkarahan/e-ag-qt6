#ifndef ABSTRACTVIDEOPREVIEW_H
#define ABSTRACTVIDEOPREVIEW_H

#include <QOpenGLWidget>
#include "../common/MediaFrame.h"

class AbstractVideoPreview : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit AbstractVideoPreview(QWidget *parent = nullptr)
        : QOpenGLWidget(parent)
    {
    }

    virtual ~AbstractVideoPreview() = default;

    virtual void onFrame(const MediaFrame &frame) = 0;
};

#endif
