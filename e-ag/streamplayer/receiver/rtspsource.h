#pragma once

#include <QObject>
#include <QThread>
#include "../network/rtppacket.h"
#include "../common/StreamSettings.h"
class RtspWorker;

class RtspSource : public QObject
{
    Q_OBJECT

public:
    explicit RtspSource(QObject *parent = nullptr);
    ~RtspSource();

    bool open(const QString &url,const StreamSettings &settings);
    void close();
    void start();

    void stop();
    RtspWorker *m_worker = nullptr;
signals:
    void startWorker();
    void stopWorker();

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

    QThread *m_thread = nullptr;

    StreamSettings  m_settings;
};
