#include "udppacketrouter.h"


UdpPacketRouter::UdpPacketRouter(QObject *parent): QObject(parent)
{
    log("UdpPacketRouter STARTED");
    getUdpSocket.bind(QHostAddress::AnyIPv4,UDP_PORT,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    getUdpSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,QVariant(false));
     QNetworkInterface iface =QNetworkInterface::interfaceFromName(detectNetworkType());
    bool ok = getUdpSocket.joinMulticastGroup( QHostAddress(MULTICAST_IP),iface);
    connect(&getUdpSocket,
            &QUdpSocket::readyRead,
            this,
            &UdpPacketRouter::processPendingDatagrams,
            Qt::UniqueConnection);
}

void UdpPacketRouter::processPendingDatagrams()
{
    if(seen.size() > 10000) seen.clear();
    while(getUdpSocket.hasPendingDatagrams())
        {
        QByteArray d;
        d.resize(getUdpSocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        getUdpSocket.readDatagram(d.data(), d.size(), &sender,
                                  &senderPort);
        //qDebug()<<"geldi:"<<sender.toString()<<getLocalIp();
        if(sender.toString()==getLocalIp()) continue;
        QDataStream s(d);
        quint32 type;
        quint64 packetId;
        s >> type>> packetId;
        QString key = QString("%1-%2-%3").arg(sender.toString()).arg(type).arg(packetId);
        ///qDebug()<<"geldi key:"<<type<<getLocalIp()<<key;
        if(seen.contains(key)) continue;
        seen.insert(key);
        ///qDebug()<<"geldiii key:"<<getLocalIp()<<key;
        switch(type)
                {
                case DISCOVERY:
                    emit discoveryReceived(d,sender);
                    break;

                case DISCOVERY_REPLY:
                    emit discoveryReplyReceived(d,sender);
                    break;

                /*case GET_FILE:
                    emit getFileDesktopRequestReceived(d,sender);
                break;

                case GET_FILE_HOME:
                    emit getFileHomeRequestReceived(d,sender);
                break;*/

                default:
                    emit transferPacketReceived(d,sender);
                    break;
                }
        }

}
