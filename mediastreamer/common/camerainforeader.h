#ifndef CAMERAINFOREADER_H
#define CAMERAINFOREADER_H

#include <QString>

struct CameraInfo
{
    bool valid = false;

    QString name;
    QString driver;
    QString bus;

    int width = 0;
    int height = 0;

    double fps = 0.0;

    QString pixelFormat;
};

class CameraInfoReader
{
public:
    static CameraInfo read(const QString &device);
};

#endif // CAMERAINFOREADER_H
