#include "videodecoderfactory.h"
#include "AbstractVideoDecoder.h"
#include "CpuVideoDecoder.h"
#include "vaapivideodecoder.h"
#include "vaapizerocopydecoder.h"
#include <QDebug>

AbstractVideoDecoder *VideoDecoderFactory::create(
    QObject *parent,
    const StreamSettings &settings)
{
    // Önce VAAPI decoder'ı dene.
  /*  auto *gpu = new VaapiZeroCopyDecoder(parent);
    gpu->setSettings(settings);

    if (gpu->start())
    {
        qDebug() << "VideoDecoderFactory: VAAPI enabled";
        return gpu;
    }

    qWarning() << "VideoDecoderFactory: VAAPI startup failed;"
                  " falling back to CPU";

    gpu->stop();
    delete gpu;

    // VAAPI başlatılamadıysa CPU decoder'ı başlat.*/


    auto *cpu = new CpuVideoDecoder(parent);
    cpu->setSettings(settings);

    if (cpu->start())
    {
        qDebug() << "VideoDecoderFactory: CPU fallback enabled";
        return cpu;
    }

    qCritical() << "VideoDecoderFactory: CPU startup failed";

    cpu->stop();
    delete cpu;

    return nullptr;
}
