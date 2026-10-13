#pragma once

#include <QObject>
#include <QThread>

#include "../common/StreamSettings.h"
#include "../network/rtppacket.h"

class RtpWorker;

class RtpSource : public QObject
{
    Q_OBJECT

public:
    explicit RtpSource(QObject *parent = nullptr);
    ~RtpSource();

    bool open(const StreamSettings &settings);

    void start();
    void stop();
    void close();
    RtpWorker *m_worker = nullptr;
signals:
    void startWorker();
    void stopWorker();

    void connected();
    void disconnected();

    void errorOccurred(const QString &error);

    void videoPacket(const RTPPacket &packet);
    void audioPacket(const RTPPacket &packet);

private:
    QThread *m_thread = nullptr;
   // RtpWorker *m_worker = nullptr;

    StreamSettings m_settings;
};
