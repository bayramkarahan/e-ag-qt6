#pragma once

#include <QObject>
#include<type.h>
class DiscoveryManager;
class TransferManager;
class FileSender;
class FileReceiver;
class MonitorServer;
class UdpPacketRouter;
class NormSender;
class NormReceiver;

class DDSService : public QObject
{
    Q_OBJECT

public:
    explicit DDSService(QObject *parent = nullptr);
    RoleInfo roleInfo;
    QMap<QString, Client> clients;
    UdpPacketRouter *udpPacketRouter;
    DiscoveryManager *discoveryManager;
    TransferManager *transferManager;
    FileSender *fileSender;
    FileReceiver *fileReceiver;
    MonitorServer *monitorServer;

    NormSender *normFileSender;
    NormReceiver *normFileReceiver;


    QString localIp;
    quint64 packetId = 0;

public slots:
    void senderFile(
            QString filePath,
            QString destPath,
            SourceType sourceType,
            TargetLocation targetLocation,
            QStringList clientList);

    void receiverFile(QString fileName, SourceType sourceType, QStringList clientList, SourceLocation sourceLocation, TargetLocation targetLocation);
    void getFileDoProcess(
            QString myrole,
            QString myfile,
            SourceType mySourceType,
            QStringList clientList,
            SourceLocation sourceLocation,
            TargetLocation targetLocation);
    QString resolveSourceRoot(SourceLocation location);
    bool copyRecursively(const QString &srcPath,const QString &dstPath);
    QString createArchive(const QStringList &sourcePaths);
    void sendSourcePaths(const QStringList &sourcePaths,SourceType sourceType,TargetLocation targetLocation,
    const QStringList &clientList,const QString &destPath);
    QStringList sourcePaths(QString sourceRoot, QString myfile,SourceType mySourceType);

private:


};
