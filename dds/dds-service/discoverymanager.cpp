#include "discoverymanager.h"

DiscoveryManager::DiscoveryManager(DDSService *service, QObject *parent)
    : QObject(parent),service(service)
{
    //qDebug() << "DiscoveryManager this =" << this;
    log("DiscoveryManager STARTED");

}

void DiscoveryManager::sendDiscovery()
{
    log("SEND DISCOVERY");
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    quint64 packetId =  QRandomGenerator::global()->generate64();
    quint64 sendTime = QDateTime::currentMSecsSinceEpoch();
    stream << (quint32)DISCOVERY;
    stream << packetId;
    stream << sendTime;

    sendSocket.writeDatagram(msg,QHostAddress(MULTICAST_IP),UDP_PORT);
}

void DiscoveryManager::receivedDiscovery(QByteArray d,QHostAddress sender)
{
    log(QString("RECEIVED DISCOVERY: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    quint64 packetId;
    quint64 sendTime;
    s>>type >> packetId>>sendTime;
   sendDiscoveryReply(sender,sendTime);
}

void DiscoveryManager::sendDiscoveryReply(QHostAddress sender,quint64 sendTime)
{
    log("SEND DISCOVERY_REPLY");
    QString clientHostName = QHostInfo::localHostName();
    quint64 packetId =  QRandomGenerator::global()->generate64();
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)DISCOVERY_REPLY;
    stream << packetId;
    stream << sendTime;
    stream << clientHostName;
    stream << getRole().role;
    stream << getRole().sendToSender;
    stream << getRole().sendToReceiver;
    stream << getRole().receiver;
    stream << getRole().getToOther;
    sendSocket.writeDatagram(msg, sender, UDP_PORT);
}

void DiscoveryManager::receivedDiscoveryReply(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED DISCOVERYREPLY: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    quint64 packetId;
    quint64 gecenSure;
    quint64 sendTime;
    QString clientHostName;
    QString role;
    bool rolesendToSender;
    bool rolesendToReceiver;
    bool rolereceiver;
    bool rolegetToOther;
    s>>type >> packetId>>sendTime>>clientHostName>>role>>rolesendToSender>>rolesendToReceiver>>rolereceiver>> rolegetToOther;
    gecenSure=QDateTime::currentMSecsSinceEpoch()-sendTime;

    QString ip = sender.toString();
    Client client;
    client.ip=ip;
    client.completed=false;
    client.hostName=clientHostName;
    client.role=role;
    client.rolesendToSender=rolesendToSender;
    client.rolesendToReceiver=rolesendToReceiver;
    client.rolereceiver=rolereceiver;
    client.rolegetToOther=rolegetToOther;
    client.discoveryTime=gecenSure;


    if(!service->clients.contains(ip))
    {
        service->clients[ip]=client;

    }else{

        service->clients[ip].completed=false;
        service->clients[ip].hostName=clientHostName;
        service->clients[ip].role=role;
        service->clients[ip].rolesendToSender=rolesendToSender;
        service->clients[ip].rolesendToReceiver=rolesendToReceiver;
        service->clients[ip].rolereceiver=rolereceiver;
        service->clients[ip].rolegetToOther=rolegetToOther;
        service->clients[ip].discoveryTime=gecenSure;
    }
        emit clientAddedMonitor(&client);
}

void DiscoveryManager::sendFileHome(QByteArray data, QHostAddress sender)
{

    QDataStream s(data);

    quint32 type;
    s >> type;   // type alanını tüket

    SourceType mySourceType;
    QString myrole;
    QString myfile;

    s >> myrole
      >> myfile
      >> mySourceType;

}


void DiscoveryManager::sendGetFile(QString myrole, QString myfile, SourceType mySourceType, QStringList clientList, SourceLocation sourceLocation,TargetLocation targetLocation)
{
    log("SEND GET_FILE");
    ///qDebug()<<"gönderilen receiver:"<<myip;
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    quint64 packetId =  QRandomGenerator::global()->generate64();
    stream << (quint32)GET_FILE;
    stream << packetId;
    stream << myrole;
    stream << myfile;
    stream << mySourceType;
    stream << clientList;
    stream << sourceLocation;
    stream << targetLocation;
    sendSocket.writeDatagram(msg,QHostAddress(MULTICAST_IP),UDP_PORT);
    qDebug()<<"sendGetFile"<<packetId<<myrole<<myfile<<(quint32)mySourceType<<clientList<<(quint32)sourceLocation<<(quint32)targetLocation;

}

