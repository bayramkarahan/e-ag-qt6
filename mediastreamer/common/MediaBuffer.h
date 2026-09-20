#ifndef MEDIABUFFER_H
#define MEDIABUFFER_H

#include <QtGlobal>
#include <QByteArray>
#include <functional>

class MediaBuffer
{
public:

    virtual ~MediaBuffer() = default;

    uchar *plane[4]{};

    int stride[4]{};

    int planeSize[4]{};

    int planeCount = 0;

    QByteArray storage;

    bool isValid() const
    {
        return planeCount > 0 &&
               plane[0] != nullptr;
    }
};
#endif
