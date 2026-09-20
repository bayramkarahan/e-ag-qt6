#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include <QObject>
#include <QAudioSource>
#include <QMediaDevices>
#include <QAudioFormat>
#include <QIODevice>
#include<QThread>
#include "sync/mastertime.h"
#include "audioframe.h"

class AudioCapture : public QObject
{
    Q_OBJECT

public:
    explicit AudioCapture(QObject *parent = nullptr);
    ~AudioCapture();
    void setMasterTime(MasterTime *masterTime);
    bool open(int sampleRate = 48000,
              int channels = 2);
    void close();

signals:
    void frameReady(const AudioFrame &frame);

private slots:
    void readData();

private:
    QAudioSource *m_source = nullptr;
    QIODevice *m_device = nullptr;

    QByteArray m_buffer;

    qint64 m_pts = 0;
    MasterTime *m_masterTime = nullptr;
};

#endif
