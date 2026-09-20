#pragma once

#include <QObject>
#include <QAtomicInt>

#include <normApi.h>
#include<QNetworkInterface>
#include "type.h"
#include <QHash>
#include<ddsservice.h>
#include<transfermanager.h>
struct NormProgressInfo
{
    qint64 startTime = 0;
    int lastProgress = -1;
};


class NormReceiverWorker : public QObject
{
    Q_OBJECT

public:
    explicit NormReceiverWorker(DDSService *service, QObject *parent = nullptr);
    QString interfaceType;
    bool allowed;

public slots:
    void stopTransfer(quint64 transferId);
    void startReceiver(QString cacheDir,
                       QString multicastIp,
                       quint16 port);

    void stop();
    QString detectNetworkType();

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
    QString getSenderIp(NormNodeHandle sender);
    QAtomicInt running;

    NormInstanceHandle instance = NORM_INSTANCE_INVALID;

    NormSessionHandle session = NORM_SESSION_INVALID;
    QHash<NormObjectHandle, quint64> objectTransferMap;
    QHash<NormObjectHandle, NormProgressInfo> progressMap;
     DDSService *service;
     QHash<NormNodeHandle, QString> senderIpMap;
     QHash<quint64, NormObjectHandle> transferObjectMap;
};
