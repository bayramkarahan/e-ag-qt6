#pragma once

#include <QObject>
#include <QThread>
#include "type.h"
#include<ddsservice.h>
#include<transfermanager.h>
class NormReceiverWorker;

class NormReceiver : public QObject
{
    Q_OBJECT

public:
    explicit NormReceiver(DDSService *service, QObject *parent = nullptr);
    ~NormReceiver();

    void start(QString cacheDir,
               QString multicastIp = "224.1.2.3",
               quint16 port = 6003);
    void stop();
    void stopTransfer(quint64 transferId);
    void setAllowed(bool allowed=true);
    void helloReply(qint64 timestamp);

signals:
    void progress(QString fileName,
                  qint64 current,
                  qint64 total,
                  double percent);

    void fileReceived(QString filePath,quint64 transferId,QString ip);

    void senderActive();

    void senderInactive();

    void error(QString text);

private:
    QThread thread;
    NormReceiverWorker *worker = nullptr;
    DDSService *service;

};
