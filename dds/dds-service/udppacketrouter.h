#ifndef UDPPACKETROUTER_H
#define UDPPACKETROUTER_H
#include<type.h>

class UdpPacketRouter: public QObject
{
        Q_OBJECT
public:
    explicit UdpPacketRouter(QObject *parent=nullptr);
    QSet<QString> seen;
private slots:
    void processPendingDatagrams();

signals:
    void datagramReceived(QByteArray d,QHostAddress sender);
    void getFileDesktopRequestReceived(QByteArray d,QHostAddress sender);
    void getFileHomeRequestReceived(QByteArray d,QHostAddress sender);
    void transferPacketReceived(QByteArray d,QHostAddress sender);
    void discoveryReplyReceived(QByteArray d,QHostAddress sender);
    void discoveryReceived(QByteArray d,QHostAddress sender);
    void metaReceived(QByteArray d,QHostAddress sender);

private:
    QUdpSocket getUdpSocket;
};

#endif // UDPPACKETROUTER_H
