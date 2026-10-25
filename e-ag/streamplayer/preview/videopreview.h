
#ifndef VIDEOPREVIEW_H
#define VIDEOPREVIEW_H

#include <QWidget>
#include <QImage>
#include <QMutex>
#include <QElapsedTimer>
#include "../converter/videoconverter.h"
#include "../common/StreamSettings.h"

class VideoPreview :  public QWidget
{
    Q_OBJECT

public:
    explicit VideoPreview(QWidget *parent = nullptr);

    void setImage(const QImage &image);
    void onFrame(const MediaFrame &frame);
    void clear();
    void setSettings(const StreamSettings &settings);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    mutable QMutex m_mutex;
    VideoConverter m_converter;
    QImage m_image;

    QElapsedTimer m_frameLimiter;
    static constexpr qint64 FRAME_INTERVAL_MS = 200; // 10 FPS
    StreamSettings  m_settings;
};

#endif
