#include "camerainforeader.h"

#include <QDebug>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

CameraInfo CameraInfoReader::read(const QString &device)
{
    CameraInfo info;

    int fd = ::open(device.toLocal8Bit().constData(),
                    O_RDWR | O_NONBLOCK);

    if (fd < 0)
    {
        qWarning() << "Cannot open camera:"
                   << device
                   << strerror(errno);

        return info;
    }

    //-----------------------------------
    // Device Information
    //-----------------------------------

    v4l2_capability cap {};

    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == 0)
    {
        info.name   = QString::fromLatin1(reinterpret_cast<char*>(cap.card));
        info.driver = QString::fromLatin1(reinterpret_cast<char*>(cap.driver));
        info.bus    = QString::fromLatin1(reinterpret_cast<char*>(cap.bus_info));
    }

    //-----------------------------------
    // Current Format
    //-----------------------------------

    v4l2_format fmt {};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fd, VIDIOC_G_FMT, &fmt) == 0)
    {
        info.width  = fmt.fmt.pix.width;
        info.height = fmt.fmt.pix.height;

        char fourcc[5];

        memcpy(fourcc,
               &fmt.fmt.pix.pixelformat,
               4);

        fourcc[4] = '\0';

        info.pixelFormat = QString(fourcc);
    }

    //-----------------------------------
    // FPS
    //-----------------------------------

    v4l2_streamparm parm {};
    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(fd, VIDIOC_G_PARM, &parm) == 0)
    {
        if (parm.parm.capture.timeperframe.numerator != 0)
        {
            info.fps =
                static_cast<double>(
                    parm.parm.capture.timeperframe.denominator) /
                parm.parm.capture.timeperframe.numerator;
        }
    }

    ::close(fd);

    info.valid = true;

    return info;
}
