#ifndef V4L2CAPTURE_H
#define V4L2CAPTURE_H

#include "../interfaces/ICapture.h"

#include <QVector>
#include<QDateTime>

#include <linux/videodev2.h>
#include <QSharedPointer>
#include "v4l2buffer.h"
#include "sync/mastertime.h"
#include "../interfaces/IVideoCapture.h"

class V4L2Capture : public IVideoCapture
{
    friend class V4L2Buffer;
public:

    V4L2Capture();

    ~V4L2Capture() override;
void setMasterTime(MasterTime *masterTime) override;
    bool open(const VideoCaptureSettings &settings) override;

    bool start() override;

    void stop() override;

    void close() override;

    bool grab(MediaFrame &frame) override;

  void release(MediaFrame &frame) override;




private:

    struct Buffer
    {
        void *start=nullptr;

        size_t length=0;
    };
    bool queueBuffers();
    bool queueBuffer(int index);

    static int xioctl(int fd,int request,void *arg);

    bool openDevice();

    bool queryCapability();

    bool choosePixelFormat();

    bool configureFormat();

    bool configureFrameRate();

    bool allocateBuffers();

    bool mapBuffers();

    bool enqueueAllBuffers();
    bool chooseFormat();
    bool setFormat();
    bool requestBuffers();


private:

    VideoCaptureSettings m_settings;

    int m_fd=-1;

    QVector<Buffer> m_buffers;

    int m_currentBuffer=-1;

    PixelFormat m_pixelFormat=PixelFormat::Unknown;

    quint32 m_v4l2PixelFormat=0;
    int m_bytesPerLine = 0;
    //QVector<QSharedPointer<V4L2Buffer>> m_frameBuffers;
      qint64 m_videoPts = 0;
      MasterTime *m_masterTime = nullptr;
      qint64 m_lastFrameUs = 0;
};

#endif
