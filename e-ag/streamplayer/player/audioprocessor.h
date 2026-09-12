#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>
#include <QByteArray>
#include<QDebug>
#include<QThread>

#include "../audio/audioframe.h"

class AudioProcessor : public QObject
{
    Q_OBJECT

public:
    explicit AudioProcessor(QObject *parent = nullptr);

    void setFrameSamples(int samples);
    void setFormat(int sampleRate,
                   int channels,
                   int sampleSize = 16);

public slots:
    void onFrame(const AudioFrame &frame);

signals:
    void frameReady(const AudioFrame &frame);

private:
    QByteArray m_buffer;

    int m_sampleRate = 48000;
    int m_channels = 2;
    int m_sampleSize = 16;

    int m_frameSamples = 960;
    qint64 m_bufferPts = -1;

};

#endif
