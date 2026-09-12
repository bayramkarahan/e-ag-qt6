#pragma once

#include <QObject>
#include <QString>
#include<QDebug>
#include<QDateTime>
extern "C"
{
#include <libavcodec/packet.h>
}
extern "C" {
#include <libavcodec/avcodec.h>
}
#include "../common/StreamSettings.h"
struct AVFormatContext;

class RtspWorker : public QObject
{
    Q_OBJECT

public:
    explicit RtspWorker(QObject *parent = nullptr);
    ~RtspWorker();

    void setUrl(const QString &url);
    void setSettings(const StreamSettings &settings);
        double m_sourceFps = 0.0;
public slots:

    void process();
    void stop();

private slots:
    void processVideoPacket(const AVPacket &packet);
    void processAudioPacket(const AVPacket &packet);

signals:

    void connected();
    void disconnected();

    void errorOccurred(const QString &error);
    void videoPacket(const QByteArray &nal,
                       qint64 pts,
                       bool marker);
        void audioPacket(const QByteArray &packet,
                       qint64 pts);
private:

    QString m_url;

    AVFormatContext *m_formatContext = nullptr;

    bool m_running = false;
    int m_videoStream = -1;
    int m_audioStream = -1;
    StreamSettings  m_settings;

    int m_frameCounter = 0;
};
