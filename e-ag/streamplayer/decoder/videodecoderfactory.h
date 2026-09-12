#ifndef VIDEODECODERFACTORY_H
#define VIDEODECODERFACTORY_H

#include <QObject>
#include "../common/StreamSettings.h"
class AbstractVideoDecoder;

class VideoDecoderFactory
{
public:

    static AbstractVideoDecoder *create(QObject *parent = nullptr);
};

#endif
