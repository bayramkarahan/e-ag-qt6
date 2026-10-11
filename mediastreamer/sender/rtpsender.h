#ifndef RTPSENDER_H
#define RTPSENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QVariant>
#include "../network/rtppacket.h"
#include<QThread>
#include "sync/mastertime.h"
#include "../common/streamenums.h"
#include <QNetworkInterface>
#include <QList>

struct Destination
{
    QHostAddress address;
    quint16 port;
};

class RtpSender : public QObject
{
    Q_OBJECT

public:

    explicit RtpSender(QObject *parent = nullptr);
    QList<Destination> m_destinations;
    bool open(const QHostAddress &group,
              quint16 port, SenderMode sendermode,
              int ttl = 1);
void setMasterTime(MasterTime *masterTime);
    void close();

public slots:

    void sendPacket(const RTPPacket &packet);
    void addDestination(const QHostAddress &addr,quint16 port);
    void removeDestination(const QHostAddress &addr,quint16 port);
    void clearDestinations();
private:

    QUdpSocket m_socket;
    SenderMode m_sendermode = SenderMode::Unicast;
    QHostAddress m_group;
    MasterTime *m_masterTime = nullptr;
    quint16 m_port = 0;
    QList<QUdpSocket*> m_multicastSockets;
    bool openMulticastInterfaces(int ttl);
};

#endif
