#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>

#include "../common/StreamSettings.h"
#include "../network/rtppacket.h"

class RtpWorker : public QObject
{
    Q_OBJECT

public:
    explicit RtpWorker(QObject *parent = nullptr);
    ~RtpWorker();

    void setSettings(const StreamSettings &settings);

public slots:
    void process();
    void stop();

private slots:
    void readVideoDatagrams();
    void readAudioDatagrams();

signals:
    void connected();
    void disconnected();

    void errorOccurred(const QString &error);

    void videoPacket(const RTPPacket &packet);
    void audioPacket(const RTPPacket &packet);

private:
    bool openSocket(QUdpSocket *socket,
                    const QHostAddress &group,
                    quint16 port);

    bool parseRtpPacket(const QByteArray &datagram,
                        RTPPacket &packet);

private:
    StreamSettings m_settings;

    QUdpSocket *m_videoSocket = nullptr;
    QUdpSocket *m_audioSocket = nullptr;

    bool m_running = false;
};
