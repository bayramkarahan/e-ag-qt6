#pragma once

#include <QObject>
#include<type.h>
#include<ddsservice.h>
#include<discoverymanager.h>
#include<transfermanager.h>
class DDSService;
class MonitorServer : public QObject
{
    Q_OBJECT

public:
    explicit MonitorServer(DDSService *service, QObject *parent=nullptr);
    QSet<QString> seen;
public slots:
    void processPendingDatagrams();
    void sendGetClientReply(QByteArray d, QHostAddress sender);
    void sendClientAddedMonitor(Client *c);
    void receivedMonitorSettings(QByteArray d, QHostAddress sender);
    void receivedMonitorReceiverData(QByteArray d, QHostAddress sender);
    void receivedMonitorSenderData(QByteArray d, QHostAddress sender);
    void sendProgressChanged(Client *c);
    void receivedDebStart(QString client);
    void receivedDebDone(QString client, QString status);
    void receivedScriptStart(QString client);
    void receivedScriptDone(QString client, QString status);
    void receivedMonitorStopTransfer(QByteArray d, QHostAddress sender);


private:
    DDSService *service;
    QUdpSocket getUdpSocket;
};
