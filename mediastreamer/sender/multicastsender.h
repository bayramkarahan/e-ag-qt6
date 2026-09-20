#ifndef MULTICASTSENDER_H
#define MULTICASTSENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QVariant>
#include "../network/rtppacket.h"

class MulticastSender : public QObject
{
    Q_OBJECT

public:

    explicit MulticastSender(QObject *parent = nullptr);

    bool open(const QHostAddress &group,
              quint16 port,
              int ttl = 1);

    void close();

public slots:

    void sendPacket(const RTPPacket &packet);

private:

    QUdpSocket m_socket;

    QHostAddress m_group;

    quint16 m_port = 0;
};

#endif
