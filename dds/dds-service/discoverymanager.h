#pragma once


#include<type.h>
#include<ddsservice.h>
class DiscoveryManager : public QObject
{
    Q_OBJECT

public:
    explicit DiscoveryManager(DDSService *service, QObject *parent=nullptr);


    void receivedDiscovery(QByteArray d, QHostAddress sender);
    void receivedDiscoveryReply(QByteArray d,QHostAddress sender);

signals:
    void clientAddedMonitor(Client *client);
    void datagramReceived(QByteArray d,QHostAddress sender);
    void fileRequestReceived(QByteArray d,QHostAddress sender);
    void transferPacketReceived(QByteArray d,QHostAddress sender);

    //void updateTotalBarText(QString mesaj);
    //void clientAllProgressStart();

    void clientDebInstallStart(QString client);
    void clientDebInstallDone(QString client, QString status);
    void clientScriptInstallStart(QString client);
    void clientScriptInstallDone(QString client, QString status);
    //void allTransferFinished();
    void transferFinished(QString sender,
                          QString receiver,
                          QString file,
                          QString id);


public slots:
    void sendDiscovery();
    void sendDiscoveryReply(QHostAddress sender, quint64 sendTime);
    void sendGetFile(QString myrole, QString myfile, SourceType mySourceType, QStringList clientList, SourceLocation sourceLocation,TargetLocation targetLocation);
    void sendFileHome(QByteArray data,QHostAddress sender);


private:
    DDSService *service;
};
