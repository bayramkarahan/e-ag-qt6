#ifndef AUDIOFRAME_H
#define AUDIOFRAME_H

#include <QByteArray>
#include <QMetaType>
struct AudioFrame
{
    QByteArray data;

    int sampleRate = 48000;
    int channels = 2;
    int sampleSize = 16;

    qint64 pts = 0;
};
Q_DECLARE_METATYPE(AudioFrame)

#endif
