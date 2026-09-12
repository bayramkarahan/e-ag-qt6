#ifndef VIDEOPREVIEW_H
#define VIDEOPREVIEW_H

#include <QObject>

#include "../converter/videoconverter.h"
#include "../renderer/videorenderer.h"
#include "../sync/mediaclock.h"
class VideoPreview : public QObject
{
    Q_OBJECT

public:

    explicit VideoPreview(VideoRenderer *renderer,
                          QObject *parent=nullptr);
void setMediaClock(MediaClock *masterTime);
public slots:

    void onFrame(const MediaFrame &frame);

private:
    MediaClock *m_clock = nullptr;
    VideoConverter m_converter;

    VideoRenderer *m_renderer=nullptr;

};

#endif
