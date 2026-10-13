#ifndef RTPRECEIVER_H
#define RTPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#include "../network/rtppacket.h"

class RTPReceiver : public QObject
{
    Q_OBJECT

public:

    explicit RTPReceiver(QObject *parent=nullptr);

    bool open(const QHostAddress &group,
              quint16 port);

    void close();

signals:

    void packetReceived(const RTPPacket &packet);

private slots:

    void readPendingDatagrams();

private:

    QUdpSocket m_socket;
};

#endif
