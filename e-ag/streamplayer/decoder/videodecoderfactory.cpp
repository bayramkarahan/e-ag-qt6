#include "videodecoderfactory.h"
#include "AbstractVideoDecoder.h"
#include "CpuVideoDecoder.h"
#include "vaapivideodecoder.h"
#include "vaapizerocopydecoder.h"
#include <QDebug>

AbstractVideoDecoder *VideoDecoderFactory::create(QObject *parent)
{
    if (VaapiVideoDecoder::isSupported())
    {
        qDebug() << "Using VAAPI decoder";

        return new VaapiZeroCopyDecoder(parent);
    }

    qDebug() << "Using CPU decoder";

    return new CpuVideoDecoder(parent);
}
