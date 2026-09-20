#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QObject>
#include <QThread>

#include <QAtomicInt>

#include "../capture/v4l2capture.h"
#include "../interfaces/IVideoCapture.h"
#include "../capture/screencapture.h"
#include "../capture/ffmpegscreencapture.h"
#include "../capture/pipewirescreencapture.h"
#include "../capture/ffmpegpipewirescreencapture.h"
#include "../capture/mediafilecapture.h"
#include "../capture/mediareaderthread.h"

class CaptureThread : public QObject
{
    Q_OBJECT

public:

    explicit CaptureThread(QObject *parent = nullptr);
    ~CaptureThread();

    bool open(const VideoCaptureSettings &settings);
void setMasterTime(MasterTime *);
    void start();
    void stop();
    IVideoCapture *capture() const
    {
        return m_capture;
    }

signals:

    void frameReady(const MediaFrame &frame);

    void error(const QString &text);
    void audioFrameReady(const AudioFrame &frame);

private slots:

    void process();

private:

    QThread m_thread;

    //V4L2Capture m_capture;
    IVideoCapture *m_capture = nullptr;

    QAtomicInt m_running{0};
    VideoCaptureSettings m_settings;
    // Sadece MediaFile için kullanılır
      MediaFileCapture *m_mediaFileCapture = nullptr;
      MediaReaderThread *m_reader = nullptr;
};

#endif
