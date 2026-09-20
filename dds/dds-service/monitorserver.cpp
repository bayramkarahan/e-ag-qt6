#include "monitorserver.h"

MonitorServer::MonitorServer(DDSService *service, QObject *parent)
    : QObject(parent),service(service)
{
    log("MonitorServer STARTED");
    getUdpSocket.bind(QHostAddress(getLocalIp()),MONITOR_CLIENT_SERVER);
    connect(&getUdpSocket,&QUdpSocket::readyRead,this,&MonitorServer::processPendingDatagrams);
}

void MonitorServer::processPendingDatagrams()
{

    if(seen.size() > 10000) seen.clear();
    while(getUdpSocket.hasPendingDatagrams())
    {
        QByteArray d;
        d.resize(getUdpSocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        getUdpSocket.readDatagram(d.data(), d.size(), &sender,&senderPort);
        //if(sender.toString()==getLocalIp()) continue;
        QDataStream s(d);
        quint32 type;
        quint64 packetId;
        s >> type;//>> packetId;
        //QString key = QString("%1-%2-%3").arg(sender.toString()).arg(type).arg(packetId);
        //if(seen.contains(key)) continue;
        //seen.insert(key);
        //qDebug()<<"type:"<<static_cast<int>(type);
        switch(type)
        {
        case MONITOR_DISCOVERY:
        {
            log("RECEIVED MONITOR_DISCOVERY");
            service->roleInfo=getRole();
            /*qDebug()<<"service->role:"<<service->roleInfo.role;
            qDebug()<<"service->role:"<<service->roleInfo.sendToSender;
            qDebug()<<"service->role:"<<service->roleInfo.sendToReceiver;
            qDebug()<<"service->role:"<<service->roleInfo.receiver;
            qDebug()<<"service->role:"<<service->roleInfo.getToOther;
            qDebug()<<"service->role:"<<service->roleInfo.root;
            */
            service->discoveryManager->sendDiscovery();

            break;
}
        case MONITOR_SENDER_DATA:
            receivedMonitorSenderData(d,sender);
            break;

        case MONITOR_RECEIVER_DATA:
            receivedMonitorReceiverData(d,sender);
            break;

        case MONITOR_SETTINGS:
            receivedMonitorSettings(d,sender);
            break;

        case MONITOR_STOP_TRANSFER:
            receivedMonitorStopTransfer(d,sender);
            break;



        default:
            //emit transferPacketReceived(d,sender);
            break;
        }
    }

}

void MonitorServer::sendProgressChanged(Client *c)
{
    //Client.
    ///log(QString("RECEIVED RECEIVER PROGRESS"));
    //qDebug()<<c->ip<<c->hostName;
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)MONITOR_PROGRESS_CHANGED;
    stream << c->packetId;
    stream << c->ip;
    stream << c->hostName;
    stream << c->fileName;
    stream << c->completed;
    stream << c->role;
    stream << c->rolesendToSender;
    stream << c->rolesendToReceiver;
    stream << c->rolereceiver;
    stream << c->rolegetToOther;
    stream << c->transferId;
    stream << c->totalSize;
    stream << c->percent;
    stream << c->received;
    stream << c->elapsedSec;
    stream << c->remainSec;
    stream << c->speedData;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_PORT);
}

void MonitorServer::sendClientAddedMonitor(Client *c){
    log(QString("SEND CLIENT RECORD"));
    //qDebug()<<c->ip<<c->hostName;
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)MONITOR_CLIENT_ADD;
        stream << (quint64)0;
        stream << c->ip;
        stream << c->hostName;
        stream << c->role;

        stream << c->rolesendToSender;
        stream << c->rolesendToReceiver;
        stream << c->rolereceiver;
        stream << c->rolegetToOther;

        stream << c->percent;
        stream << c->completed;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_PORT);
}

void MonitorServer::receivedMonitorSenderData(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED MONITOR_SENDER_DATA: %1").arg(sender.toString()));
    QString filePath;
    QString destPath;
    SourceType sourceType;
    QStringList clientList;
    TargetLocation transferLocation;
    QDataStream s(d);
    quint32 type;
    s>>type>>filePath>>destPath>>sourceType>>transferLocation>>clientList;
     QString ip = sender.toString();
    //qDebug()<<"receivedMonitorSenderData"<< filePath<<destPath<< static_cast<int>(transferLocation);
    //qDebug()<<"seçili client liste:"<<clientList;
    service->senderFile(filePath,destPath,sourceType,transferLocation,clientList);
}

void MonitorServer::receivedMonitorReceiverData(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED MONITOR_RECEIVER_DATA: %1").arg(sender.toString()));
    QString fileName;
    QStringList clientList;
    SourceType sourceType;
    SourceLocation sourceLocation;
    TargetLocation targetLocation;
    QDataStream s(d);
    quint32 type;
    s>>type>>fileName>>sourceType>>sourceLocation>>targetLocation>>clientList;
     QString ip = sender.toString();
 //qDebug()<<"gelecekler"<<clientList;
   // qDebug()<<"receivedMonitorReceiverData"<< fileName<< static_cast<int>(sourceType);

    service->receiverFile(fileName,sourceType,clientList,sourceLocation,targetLocation);
}

void MonitorServer::receivedMonitorSettings(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED MONITOR_SETTINGS: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    s>>type;
    RoleInfo ri;
    s>> ri.role;
    s>> ri.sendToSender;
    s>> ri.sendToReceiver;
    s>> ri.receiver;
    s>> ri.getToOther;
    s>> ri.root;
    /*qDebug()<<"role"<<ri.role;
    qDebug()<<"sendToSender"<<ri.sendToSender;
    qDebug()<<"sendToReceiver"<<ri.sendToReceiver;
    qDebug()<<"receiver"<<ri.receiver;
    qDebug()<<"getToOther"<<ri.getToOther;
    qDebug()<<"root"<<ri.root;*/

    QProcess p;

         p.start( "/usr/bin/dds-conf",
                  {"set-role", ri.role,
                  ri.sendToSender ? "true" : "false",
                  ri.sendToReceiver ? "true" : "false",
                  ri.receiver ? "true" : "false",
                  ri.getToOther? "true" : "false",
                  ri.root? "true" : "false"});

         p.waitForFinished();

         if(p.exitCode() == 0)
         {
             qDebug() << "Başarılı: Kaydedildi.";
             quint64 gecenSure=QDateTime::currentMSecsSinceEpoch()-7;
             service->discoveryManager->sendDiscoveryReply(QHostAddress(MULTICAST_IP),gecenSure);
         }
         else
         {
             qDebug() << "Hata: kaydedilmedi";
         }

}

void MonitorServer::receivedMonitorStopTransfer(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED MONITOR_STOP_TRANSFER: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    quint64 transferId;
    QStringList clientList;
    s>>type;
    s>> transferId;
    s>> clientList;
    //qDebug()<<"transferId"<<transferId;
    //qDebug()<<"clientList"<<clientList;
    service->transferManager->sendStopTransfer(transferId,clientList);
}

void MonitorServer::sendGetClientReply(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED  GET_CLIENTS: %1").arg(sender.toString()));
    log(QString("SEND GET_CLIENTS_REPLY: %1").arg(sender.toString()));
    QUdpSocket sendSocket;
    //sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)MONITOR_DISCOVERY_REPLY;
    stream << (quint32)service->clients.size();

    for(auto it = service->clients.begin();
         it != service->clients.end();
         ++it)
    {
        Client *c = &it.value();
        stream << (quint64)0;
        stream << c->ip;
        stream << c->hostName;
        stream << c->role;

        stream << c->rolesendToSender;
        stream << c->rolesendToReceiver;
        stream << c->rolereceiver;
        stream << c->rolegetToOther;

        stream << c->percent;
        stream << c->completed;
    }
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_PORT);
}

void MonitorServer::receivedDebStart(QString client)
{
    log(QString("SEND TO MONITOR DEB_START"));
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)DEB_START;
    stream << client;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_PORT);
}

void MonitorServer::receivedDebDone(QString client, QString status)
{
    log(QString("SEND TO MONITOR DEB_DONE"));
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)DEB_DONE;
    stream << client;
    stream << status;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_PORT);

}

void MonitorServer::receivedScriptStart(QString client)
{
    log(QString("SEND TO MONITOR SCRIPT_START"));
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)SCRIPT_START;
    stream << client;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_PORT);

}

void MonitorServer::receivedScriptDone(QString client, QString status)
{
    log(QString("SEND TO MONITOR SCRIPT_DONE"));
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)SCRIPT_DONE;
    stream << client;
    stream << status;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_PORT);

}
