#ifndef UDPSERVICE_H
#define UDPSERVICE_H
#include<type.h>
#include<monitorclient.h>

void MonitorClient::updConnect()
{
    qDebug()<<"updConnect";
    getUdpSocket.bind(QHostAddress(getLocalIp()), MONITOR_CLIENT_PORT);
    connect(&getUdpSocket,&QUdpSocket::readyRead,this, &MonitorClient::processPendingDatagrams);
    qDebug()<<"updConnect1";
}

void MonitorClient::processPendingDatagrams()
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

        switch(type)
        {
        case MONITOR_DISCOVERY_REPLY:
            receivedMonitorDiscoveryReply(d,sender);
            break;
        case MONITOR_CLIENT_ADD:
            receivedClientAdd(d,sender);
            break;
        case MONITOR_PROGRESS_CHANGED:
            receivedMonitorProgress(d,sender);
            break;
        case DEB_START:
            receivedDebStart(d,sender);
            break;
        case DEB_DONE:
            receivedDebDone(d,sender);
            break;
        case SCRIPT_START:
            receivedScriptStart(d,sender);
            break;
        case SCRIPT_DONE:
            receivedScriptDone(d,sender);
            break;

        default:
            //emit transferPacketReceived(d,sender);
            break;
        }
    }

}

void MonitorClient::sendSenderData(QString destPath, TargetLocation targetLocation)
{

    if(isUserSpecialDirectory(filePathLE->text()))
    {
        QMessageBox::warning(this,"Uyarı","Bu klasör doğrudan paylaşılamaz.");
        return;
    }

    log(QString("SEND MONITOR_SENDER_DATA"));

    QStringList clientList;
    for(ClientRow *client : clients)
    {
        if(client->select->isChecked())
        {
            clientList<<client->ip->text();
        }
    }
///qDebug()<<"clientList"<<clientList;

    SourceType sourceType;

    allProgressStart(0);
     QTimer::singleShot(10, [this,destPath,sourceType,targetLocation,clientList](){
         QUdpSocket sendSocket;
         sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
         QByteArray msg;
         QDataStream stream(&msg, QIODevice::WriteOnly);
         stream << (quint32)MONITOR_SENDER_DATA;
         stream << filePathLE->text();
         stream << destPath;
         stream << (quint32)sourceType;
         stream << (quint32)targetLocation;
         stream << clientList;
         sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_SERVER);
         //qDebug()<<"udpservice"<< filePathLE->text()<<destPath<< static_cast<int>(transferLocation);

     });

}

void MonitorClient::sendStopTransfer(quint64 transferId,QString ip)
{


    log(QString("SEND MONITOR_STOP_TRANSFER"));

    QStringList clientList;
    if(ip=="")
    {
    for(ClientRow *client : clients)
    {
        if(client->select->isChecked())
        {

        }
    }
    }else

    if(clientList.empty()) clientList<<ip;
qDebug()<<"clientList"<<clientList<< transferId<< ip;

         QUdpSocket sendSocket;
         sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
         QByteArray msg;
         QDataStream stream(&msg, QIODevice::WriteOnly);
         stream << (quint32)MONITOR_STOP_TRANSFER;
         stream <<transferId;
         stream << clientList;
         sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_SERVER);
         //qDebug()<<"udpservice"<< filePathLE->text()<<destPath<< static_cast<int>(transferLocation);


}


void MonitorClient::sendStopTransferAll()
{


    log(QString("SEND MONITOR_STOP_TRANSFER"));

    QMap<quint64, QSet<QString>> transferIdMap;

    for (ClientRow *client : clients)
    {
        if (!client->select->isChecked())
            continue;

        transferIdMap[client->transferId].insert(client->ip->text());
    }



    for (auto it = transferIdMap.begin(); it != transferIdMap.end(); ++it)
    {
        quint64 transferId = it.key();
        QStringList clientList = it.value().values();
        qDebug() << "Transfer ID:" << transferId<<clientList;

        QUdpSocket sendSocket;
        sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
        QByteArray msg;
        QDataStream stream(&msg, QIODevice::WriteOnly);
        stream << (quint32)MONITOR_STOP_TRANSFER;
        stream <<transferId;
        stream << clientList;
        sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_SERVER);

    }




}


void MonitorClient::sendReceiverData(QString fileName, SourceType sourceType,SourceLocation sourceLocation,TargetLocation targetLocation)
{
    QStringList clientList;
    for(ClientRow *client : clients)
    {
        if(client->select->isChecked())
        {
            clientList<<client->ip->text();
        }
    }
//qDebug()<<"clientList"<<clientList;

    allProgressStart(0);

    log(QString("SEND MONITOR_RECEIVER_DATA"));
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)MONITOR_RECEIVER_DATA;
    stream << fileName;
    stream << (quint32)sourceType;
    stream << (quint32)sourceLocation;
    stream << (quint32)targetLocation;
    stream << clientList;

    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_SERVER);
//qDebug()<<"data gitti"<<(quint32)MONITOR_RECEIVER_DATA<<filePathLE->text()<<destPath<<(quint32)transferLocation;;
}

void MonitorClient::receivedMonitorProgress(QByteArray d, QHostAddress sender)
{
    //log(QString("RECEIVED PROGRESS: %1").arg(sender.toString()));

    QDataStream s(d);
    quint32 type;
    s>>type;
     QString ip = sender.toString();
    Client *c=new Client;
    s>> c->packetId;
    s>> c->ip;
    s>> c->hostName;
    s>> c->fileName;
    s>> c->completed;
    s>> c->role;
    s>> c->rolesendToSender;
    s>> c->rolesendToReceiver;
    s>> c->rolereceiver;
    s>> c->rolegetToOther;
    s>> c->transferId;
    s>> c->totalSize;
    s>> c->percent;
    s>>c->received;
    s>> c->elapsedSec;
    s>> c->remainSec;
    s>> c->speedData;
       /* qDebug()<< c.ip<< c.hostName<< c.role;*/
        updateProgress(c->transferId,c->ip,c->received,c->totalSize,c->percent,c->hostName,c->role,c->rolesendToSender,
                       c->rolesendToReceiver,c->rolereceiver,c->rolegetToOther);
}

void MonitorClient::receivedClientAdd(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED CLIENT ADDED: %1").arg(sender.toString()));

    QDataStream s(d);
    quint32 type;
    s>>type;
    //>>clientHostName>>role>>rolesendToSender>>rolesendToReceiver>>rolereceiver>> rolegetToOther;
    QString ip = sender.toString();
        Client c;
        s >> c.transferId;
        s >> c.ip;
        s >> c.hostName;
        s >> c.role;
        s >> c.rolesendToSender;
        s >> c.rolesendToReceiver;
        s >> c.rolereceiver;
        s >> c.rolegetToOther;
        s >> c.percent;
        s >> c.completed;

       /* qDebug()<< c.ip<< c.hostName<< c.role;*/
        updateProgress(c.transferId,c.ip,0,0,0,c.hostName,c.role,c.rolesendToSender,
                       c.rolesendToReceiver,c.rolereceiver,c.rolegetToOther);
}

void MonitorClient::sendMonitorDiscovery()
{
    log(QString("SEND MONITOR_DISCOVERY"));
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)MONITOR_DISCOVERY;
    sendSocket.writeDatagram(msg,QHostAddress(getLocalIp()),MONITOR_CLIENT_SERVER);

}

void MonitorClient::sendRoleStatus(QString ip)
{
    log(QString("SEND MONITOR_ROLE_STATUS"));
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    for(ClientRow *client : clients)
    {
        if(client->ip->text()==ip)
        {
            QByteArray msg;
            QDataStream stream(&msg, QIODevice::WriteOnly);
            quint64 packetId =  QRandomGenerator::global()->generate64();
            stream << (quint32)MONITOR_ROLE_STATUS;
            stream << packetId;
            //stream << quint32(client.size());
            //client->sendToSender->setChecked(checked);
            qDebug()<<"mesaj:"<<client->ip->text()
                   <<client->role->text()
              <<(client->sendToSender->checkState()==Qt::Checked)
              <<(client->sendToReceiver->checkState()==Qt::Checked)
              <<(client->receiver->checkState()==Qt::Checked)
              <<(client->getToOther->checkState()==Qt::Checked);

            stream << (client->role->text());
            stream << (client->sendToSender->checkState()==Qt::Checked);
            stream << (client->sendToReceiver->checkState()==Qt::Checked);
            stream << (client->receiver->checkState()==Qt::Checked);
            stream << (client->getToOther->checkState()==Qt::Checked);
            stream << false;
            sendSocket.writeDatagram(msg,QHostAddress(client->ip->text()),UDP_PORT);
        }
    }
}

void MonitorClient::sendRoleStatusAll()
{
    log(QString("SEND MONITOR_ROLE_STATUSALL"));
    QUdpSocket sendSocket;
    sendSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption,0);
    for(ClientRow *client : clients)
    {
        ///if(client->ip->text()==ip)
        //{
            QByteArray msg;
            QDataStream stream(&msg, QIODevice::WriteOnly);
            quint64 packetId =  QRandomGenerator::global()->generate64();
            stream << (quint32)MONITOR_ROLE_STATUS;
            stream << packetId;
            //stream << quint32(client.size());
            //client->sendToSender->setChecked(checked);
            qDebug()<<"mesaj:"<<client->ip->text()
                   <<client->role->text()
              <<(client->sendToSender->checkState()==Qt::Checked)
              <<(client->sendToReceiver->checkState()==Qt::Checked)
              <<(client->receiver->checkState()==Qt::Checked)
              <<(client->getToOther->checkState()==Qt::Checked);

            stream << (client->role->text());
            stream << (client->sendToSender->checkState()==Qt::Checked);
            stream << (client->sendToReceiver->checkState()==Qt::Checked);
            stream << (client->receiver->checkState()==Qt::Checked);
            stream << (client->getToOther->checkState()==Qt::Checked);
            stream << false;
            sendSocket.writeDatagram(msg,QHostAddress(client->ip->text()),UDP_PORT);
        //}
    }
}

void MonitorClient::receivedMonitorDiscoveryReply(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED GET_CLIENTS_REPLY: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    quint32 clientSize;
    QString clientHostName;
    QString role;
    bool rolesendToSender;
    bool rolesendToReceiver;
    bool rolereceiver;
    bool rolegetToOther;
    s>>type >> clientSize;//>>clientHostName>>role>>rolesendToSender>>rolesendToReceiver>>rolereceiver>> rolegetToOther;
    QString ip = sender.toString();
    //qDebug()<<clientSize;
    for(quint32 i=0;i<clientSize;i++)
    {
        Client c;
        s >> c.transferId;
        s >> c.ip;
        s >> c.hostName;
        s >> c.role;

        s >> c.rolesendToSender;
        s >> c.rolesendToReceiver;
        s >> c.rolereceiver;
        s >> c.rolegetToOther;

        s >> c.percent;
        s >> c.completed;

       /* qDebug()
            << c.ip
            << c.hostName
            << c.role;*/
        updateProgress(c.transferId,c.ip,0,0,0,c.hostName,c.role,c.rolesendToSender,
                       c.rolesendToReceiver,c.rolereceiver,c.rolegetToOther);
    }

}

void MonitorClient::receivedDebStart(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED DEB_START: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    QString clientIp;
    s>>type>>clientIp;
    QString ip = sender.toString();
    markDebInstallStart(clientIp);
 }


void MonitorClient::receivedDebDone(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED DEB_DONE: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    QString clientIp;
    QString status;
    s>>type>>clientIp>>status;
    QString ip = sender.toString();
    markDebInstallDone(clientIp,status);
 }


void MonitorClient::receivedScriptStart(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED SCRIPT_START: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    QString clientIp;
    s>>type>>clientIp;
    QString ip = sender.toString();
    markScriptInstallStart(clientIp);
 }


void MonitorClient::receivedScriptDone(QByteArray d, QHostAddress sender)
{
    log(QString("RECEIVED SCRIPT_DONE: %1").arg(sender.toString()));
    QDataStream s(d);
    quint32 type;
    QString clientIp;
    QString status;
    s>>type>>clientIp>>status;
    QString ip = sender.toString();
    markScriptInstallDone(clientIp,status);
 }

#endif // UDPSERVICE_H
