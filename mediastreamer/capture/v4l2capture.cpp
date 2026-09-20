#include "v4l2capture.h"
#include <QDebug>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/mman.h>

V4L2Capture::V4L2Capture()
{
}

V4L2Capture::~V4L2Capture()
{
    close();
}

int V4L2Capture::xioctl(int fd, int request, void *arg)
{
    int ret;

    do
    {
        ret = ::ioctl(fd, request, arg);

    } while (ret == -1 && errno == EINTR);

    return ret;
}

bool V4L2Capture::open(const VideoCaptureSettings &settings)
{
 m_settings = settings;
 qDebug()<<"camera settings "<<
           //m_settings.VideoCaptureSettings.width<<
           // m_settings.VideoCaptureSettings.height<<
             m_settings.width<<
             m_settings.height<<
           m_settings.fps;

m_videoPts = 0;
    if(!openDevice())
        return false;

    if(!queryCapability())
        return false;

    if(!chooseFormat())
        return false;

    if(!setFormat())
        return false;

    if(!requestBuffers())
        return false;

    if(!mapBuffers())
        return false;

    if(!queueBuffers())
        return false;

    return true;
}

bool V4L2Capture::openDevice()
{
    m_fd = ::open(m_settings.device.toLocal8Bit().constData(),
                  O_RDWR | O_NONBLOCK);

    if(m_fd < 0)
    {
        qCritical() << "Cannot open camera:"
                    << m_settings.device
                    << strerror(errno);

        return false;
    }

    return true;
}


bool V4L2Capture::queryCapability()
{
    struct v4l2_capability cap;

    memset(&cap,0,sizeof(cap));

    if(xioctl(m_fd,
              VIDIOC_QUERYCAP,
              &cap) < 0)
    {
        qCritical() << "VIDIOC_QUERYCAP failed:"
                    << strerror(errno);

        return false;
    }

    qDebug() << "Driver :" << reinterpret_cast<char*>(cap.driver);
    qDebug() << "Card   :" << reinterpret_cast<char*>(cap.card);
    qDebug() << "Bus    :" << reinterpret_cast<char*>(cap.bus_info);

    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {
        qCritical() << "Device is not a video capture device.";

        return false;
    }

    if(!(cap.capabilities & V4L2_CAP_STREAMING))
    {
        qCritical() << "Streaming I/O is not supported.";

        return false;
    }

    return true;
}


bool V4L2Capture::chooseFormat()
{
    v4l2_fmtdesc fmt;
    memset(&fmt, 0, sizeof(fmt));

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.index = 0;

    bool hasNV12 = false;
    bool hasYUYV = false;
    bool hasUYVY = false;

    while (xioctl(m_fd, VIDIOC_ENUM_FMT, &fmt) == 0)
    {
        qDebug() << "Format:"
                 << QString::fromLatin1(reinterpret_cast<char*>(fmt.description))
                 << QString(" (0x%1)").arg(fmt.pixelformat,8,16,QChar('0'));

        switch (fmt.pixelformat)
        {
        case V4L2_PIX_FMT_NV12:
            hasNV12 = true;
            break;

        case V4L2_PIX_FMT_YUYV:
            hasYUYV = true;
            break;

        case V4L2_PIX_FMT_UYVY:
            hasUYVY = true;
            break;

        default:
            break;
        }

        fmt.index++;
    }

    if(hasNV12)
    {
        m_pixelFormat = PixelFormat::NV12;
        m_v4l2PixelFormat = V4L2_PIX_FMT_NV12;
    }
    else if(hasYUYV)
    {
        m_pixelFormat = PixelFormat::YUYV;
        m_v4l2PixelFormat = V4L2_PIX_FMT_YUYV;
    }
    else if(hasUYVY)
    {
        m_pixelFormat = PixelFormat::UYVY;
        m_v4l2PixelFormat = V4L2_PIX_FMT_UYVY;
    }
    else
    {
        qCritical() << "Supported pixel format not found.";
        return false;
    }

    qDebug() << "Selected pixel format:"
             << QString("0x%1").arg(m_v4l2PixelFormat,8,16,QChar('0'));

    return true;
}

bool V4L2Capture::setFormat()
{
    v4l2_format format;
    memset(&format,0,sizeof(format));

    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    format.fmt.pix.width  = m_settings.width;
    format.fmt.pix.height = m_settings.height;
    format.fmt.pix.pixelformat = m_v4l2PixelFormat;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    if(xioctl(m_fd, VIDIOC_S_FMT, &format) < 0)
    {
        qCritical() << "VIDIOC_S_FMT failed:"
                    << strerror(errno);

        return false;
    }

    // Sürücünün kabul ettiği gerçek değerleri geri al
    m_settings.width  = format.fmt.pix.width;
    m_settings.height = format.fmt.pix.height;

    m_bytesPerLine = format.fmt.pix.bytesperline;

    qDebug() << "Camera format:";
    qDebug() << " Resolution :" << m_settings.width
             << "x"
             << m_settings.height;
    qDebug() << " Bytes/Line :" << m_bytesPerLine;
    qDebug() << " Image Size :" << format.fmt.pix.sizeimage;

    v4l2_streamparm parm;
    memset(&parm,0,sizeof(parm));

    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parm.parm.capture.timeperframe.numerator = 1;
    parm.parm.capture.timeperframe.denominator = m_settings.fps;

    if(xioctl(m_fd, VIDIOC_S_PARM, &parm) < 0)
    {
        qWarning() << "VIDIOC_S_PARM failed:"
                   << strerror(errno);
    }
    else
    {
        qDebug() << "Actual FPS:"
                 << parm.parm.capture.timeperframe.denominator
                 << "/"
                 << parm.parm.capture.timeperframe.numerator;
    }

    return true;
}

bool V4L2Capture::requestBuffers()
{
    v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (xioctl(m_fd, VIDIOC_REQBUFS, &req) < 0)
    {
        qCritical() << "VIDIOC_REQBUFS failed:"
                    << strerror(errno);

        return false;
    }

    if (req.count < 2)
    {
        qCritical() << "Not enough buffers allocated.";

        return false;
    }

    m_buffers.resize(req.count);

    qDebug() << "Allocated Buffers :" << req.count;

    return true;
}

bool V4L2Capture::mapBuffers()
{
    for(int i=0;i<m_buffers.size();i++)
    {
        v4l2_buffer buf;
        memset(&buf,0,sizeof(buf));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if(xioctl(m_fd,VIDIOC_QUERYBUF,&buf) < 0)
        {
            qCritical() << "VIDIOC_QUERYBUF failed:"
                        << strerror(errno);

            return false;
        }

        m_buffers[i].length = buf.length;

        m_buffers[i].start =
                mmap(nullptr,
                     buf.length,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     m_fd,
                     buf.m.offset);

        if(m_buffers[i].start == MAP_FAILED)
        {
            qCritical() << "mmap failed:"
                        << strerror(errno);

            return false;
        }

        qDebug() << "Buffer"
                 << i
                 << "mapped"
                 << m_buffers[i].length
                 << "bytes";
    }
/*
    m_frameBuffers.clear();

    for(int i = 0; i < m_buffers.size(); ++i)
    {
        auto buffer = QSharedPointer<V4L2Buffer>::create();

        buffer->index = i;

        m_frameBuffers.append(buffer);
    }
*/
    return true;
}


bool V4L2Capture::start()
{

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    m_videoPts = 0;
    if(xioctl(m_fd,VIDIOC_STREAMON,&type) < 0)

    {
        qCritical() << "VIDIOC_STREAMON failed:"
                    << strerror(errno);

        return false;
    }

    qDebug() << "Camera streaming started.";

    return true;
}

bool V4L2Capture::grab(MediaFrame &frame)
{
    //qDebug() << "Camera master =" << m_masterTime;

    static bool firstGrab = true;

    if (firstGrab)
    {
        firstGrab = false;
        qDebug() << "V4L2 grab entered";
        qDebug()<<"camera settings "<<
                  m_settings.width<<
                  m_settings.height<<
                  m_settings.fps;
    }
    pollfd pfd;
    pfd.fd = m_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    int ret = ::poll(&pfd, 1, 2000);
//qDebug() << "poll returned";
    if(ret < 0)
    {
        qCritical() << "poll failed:" << strerror(errno);
        return false;
    }

    if(ret == 0)
    {
        qWarning() << "Camera timeout.";
        return false;
    }

    v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if(xioctl(m_fd, VIDIOC_DQBUF, &buf) < 0)
    {
        if(errno == EAGAIN)
            return false;

        qCritical() << "VIDIOC_DQBUF failed:"
                    << strerror(errno);

        return false;
    }
/**********************************/
    if (m_settings.fps > 0)
    {
        qint64 now =
            m_masterTime ? m_masterTime->nowUs()
                         : QDateTime::currentMSecsSinceEpoch() * 1000;

        qint64 interval = 1000000LL / m_settings.fps;

        if (m_lastFrameUs != 0 &&
            (now - m_lastFrameUs) < interval)
        {
            queueBuffer(buf.index);
            return false;
        }

        m_lastFrameUs = now;
    }
    /***************************************/
    m_currentBuffer = buf.index;

    frame.width  = m_settings.width;
    frame.height = m_settings.height;
    frame.format = m_pixelFormat;

    if(m_masterTime)
        frame.pts = m_masterTime->nowUs();
    else
        frame.pts = 0;

    frame.keyFrame = false;
    QSharedPointer<V4L2Buffer> mediaBuffer(
        new V4L2Buffer,
        [this](V4L2Buffer *buffer)
        {
            if(m_fd >= 0)
                queueBuffer(buffer->index);

            delete buffer;
        });
    mediaBuffer->index = buf.index;

    switch(m_pixelFormat)
    {
    case PixelFormat::YUYV:
{
        mediaBuffer->planeCount = 1;

        mediaBuffer->plane[0] =
                static_cast<uchar*>(m_buffers[buf.index].start);

        mediaBuffer->stride[0] = m_bytesPerLine;

        mediaBuffer->planeSize[0] = buf.bytesused;

        break;
}
    case PixelFormat::NV12:
    {
        mediaBuffer->planeCount = 2;

        uchar *base =
                static_cast<uchar*>(m_buffers[buf.index].start);

        mediaBuffer->plane[0] = base;
        mediaBuffer->plane[1] = base + (m_settings.width * m_settings.height);

        mediaBuffer->stride[0] = m_bytesPerLine;
        mediaBuffer->stride[1] = m_bytesPerLine;

        mediaBuffer->planeSize[0] =
                m_settings.width * m_settings.height;

        mediaBuffer->planeSize[1] =
                buf.bytesused - mediaBuffer->planeSize[0];

        break;
    }

    default:

        qWarning() << "Unsupported pixel format.";

        queueBuffer(buf.index);

        return false;
    }

    frame.buffer = mediaBuffer;

    return true;
}

void V4L2Capture::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
}

void V4L2Capture::release(MediaFrame &frame)
{
   /* auto buffer =
            qSharedPointerDynamicCast<V4L2Buffer>(frame.buffer);

    if(!buffer)
        return;

    if(m_fd < 0)
        return;

    queueBuffer(buffer->index);

    frame.buffer.clear();*/
}
void V4L2Capture::stop()
{
    if(m_fd < 0)
        return;

    v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(xioctl(m_fd, VIDIOC_STREAMOFF, &type) < 0)
    {
        qWarning() << "VIDIOC_STREAMOFF failed:"
                   << strerror(errno);
    }
}

void V4L2Capture::close()
{
    if(m_fd < 0)
        return;

    stop();

    for(Buffer &buffer : m_buffers)
    {
        if(buffer.start)
        {
            munmap(buffer.start, buffer.length);

            buffer.start = nullptr;
            buffer.length = 0;
        }
    }

    m_buffers.clear();
    if(m_fd >= 0)
    {
        ::close(m_fd);
        m_fd = -1;
    }

}
bool V4L2Capture::queueBuffer(int index)
{
    if(m_fd < 0)
        return false;

    v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = index;

    if(xioctl(m_fd, VIDIOC_QBUF, &buf) < 0)
    {
        qCritical() << "VIDIOC_QBUF failed:"
                    << strerror(errno);

        return false;
    }

    return true;
}

bool V4L2Capture::queueBuffers()
{
    for(int i = 0; i < m_buffers.size(); ++i)
    {
        if(!queueBuffer(i))
            return false;
    }

    return true;
}
