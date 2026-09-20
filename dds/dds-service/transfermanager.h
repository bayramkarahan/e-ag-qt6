#pragma once

#include <QObject>
#include <QHash>

#include "type.h"
#include<ddsservice.h>
#include<monitorserver.h>
#include "normsender.h"
#include "normreceiver.h"
#include "filereceiver.h"
class TransferManager : public QObject
{
    Q_OBJECT

public:

explicit TransferManager(DDSService *service, QObject *parent=nullptr);


public slots:
    void processDatagram(QByteArray data,QHostAddress sender);

    void onReceiveFinished(quint64 transferId, QString senderIp,QString filename);
    void onSendFinished(quint64 transferId, QString senderIp,QString filename);

    void receivedMeta(QDataStream &s, QHostAddress &sender);
    void receivedDone(QDataStream &s, QHostAddress &sender);
    void receivedProgress(QDataStream &s, QHostAddress &sender);
    void receivedMonitorRoleStatus(QDataStream &s, QHostAddress &sender);
    void receivedDebStart(QDataStream &s, QHostAddress &sender);
    void receivedDebDone(QDataStream &s, QHostAddress &sender);
    void receivedScriptStart(QDataStream &s, QHostAddress &sender);
    void receivedScriptDone(QDataStream &s, QHostAddress &sender);
    void receivedDoDoneProcess(QDataStream &s, QHostAddress &sender);
    void receivedMetaReply(QDataStream &s, QHostAddress &sender);
    void receivedStopTransfer(QDataStream &s, QHostAddress &sender);

    void sendProgressChanged(Client *c);
    void sendDoDoneProcess(quint64 transferId,const QHostAddress &sender);
    void sendMeta(FileMeta fileMeta);
    void sendMetaReply(FileMeta fileMeta, const QHostAddress sender);
    void receivedGetFile(QDataStream &s, QHostAddress &sender);
    bool copyFile(const QString& src, const QString& dstDir, bool overwrite);
    bool copyDirectory(const QString &sourceDir,const QString &targetDir,
                       bool overwrite);
    void setPermissionsRecursive(const QString &path, uid_t uid, gid_t gid);
    void sendDebInstallStart(quint64 tid, const QHostAddress &sender);
    void sendDebInstallDone(QString status, const quint64 tid, const QHostAddress &sender);
    void sendScriptInstallStart(quint64 tid, const QHostAddress &sender);
    void sendScriptInstallDone(QString status,quint64 tid, const QHostAddress &sender);
    void sendDone(quint64 transferId, const QHostAddress &sender, QString comment);
    void doDoneProcess(quint64 transferId,const QHostAddress &sender);
    void normFileReceived(QString filePath,quint64 transferId,QString ip);
    void sendStopTransfer(quint64 transferId, QStringList clientList);
    void sendFileStart(quint64 tid, QString ip);
signals:
    void startFileSender(QHostAddress &sender,FileMeta fileMeta, quint16 port,QString savePath,quint64 discoveryTime);
    void debStart(QString client);
    void debDone(QString client, QString status);
    void scriptStart(QString client);
    void scriptDone(QString client, QString status);
    void transferFinished(QString sender, QString receiver,QString file,QString id);
    void clientProgressChanged(QString client,qint64 received,qint64 total,
                               int percent,QString clientHostName,QString role,bool rolesendToSender
                               ,bool rolesendToReceiver,bool rolereceiver,bool rolegetToOther);
private:
    DDSService *service;
};
