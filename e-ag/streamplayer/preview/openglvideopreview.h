#ifndef OPENGLVIDEOPREVIEW_H
#define OPENGLVIDEOPREVIEW_H

#include <QObject>

#include "../renderer/openglvideorenderer.h"
#include "../sync/mediaclock.h"

class OpenGLVideoPreview : public QObject
{
    Q_OBJECT

public:
    explicit OpenGLVideoPreview(OpenGLVideoRenderer *renderer,
                                QObject *parent = nullptr);

    void setMediaClock(MediaClock *clock);

public slots:
    void onFrame(const MediaFrame &frame);

private:
    MediaClock *m_clock = nullptr;

    OpenGLVideoRenderer *m_renderer = nullptr;
};

#endif // OPENGLVIDEOPREVIEW_H
