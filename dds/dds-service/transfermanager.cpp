#include "transfermanager.h"

TransferManager::TransferManager(DDSService *service,QObject *parent)
    : QObject(parent),
      service(service)
{
    log("TransferManager STARTED");
}

void TransferManager::processDatagram(QByteArray data,QHostAddress sender)
{
    QDataStream s(data);

    quint32 type;
    s >> type;

    switch(type)
    {
    case META:
        receivedMeta(s,sender);
        break;

    case META_REPLY:
        receivedMetaReply(s,sender);
        break;

    case GET_FILE:
        receivedGetFile(s,sender);
        break;

    case STOP_TRANSFER:
        receivedStopTransfer(s,sender);
        break;

    case DONE:
        receivedDone(s,sender);
        break;

    case DO_DONE_PROCESS:
        receivedDoDoneProcess(s,sender);
        break;

    case PROGRESS:
        receivedProgress(s,sender);
        break;

    case MONITOR_ROLE_STATUS:
        receivedMonitorRoleStatus(s,sender);
        break;

    case DEB_START:
        receivedDebStart(s,sender);
        break;

    case DEB_DONE:
        receivedDebDone(s,sender);
        break;

    case SCRIPT_START:
        receivedScriptStart(s,sender);
        break;

    case SCRIPT_DONE:
        receivedScriptDone(s,sender);
        break;

    default:
        break;
    }



}
void TransferManager::sendMetaReply(FileMeta fileMeta, const QHostAddress sender)
{
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)META_REPLY;
    stream << fileMeta.transferId;
    sendSocket.writeDatagram(msg,sender,UDP_PORT);
    log("SEND META_REPLY: " + fileMeta.sourcePath+" id:"+QString::number(fileMeta.transferId));
    //qDebug()<< fileMeta.sourceBaseName<<fileMeta.transferOverwrite;
}

void TransferManager::receivedMeta(QDataStream &s,QHostAddress &sender) {
    QString ip = sender.toString();
    if(getLocalIp()==ip) return;
    FileMeta meta;
    s >> meta.transferId;
    s >> meta.sourcePath;
    s >> meta.targetTempPath;
    s >> meta.targetDestinationPath;
    s >> meta.targetLocation;
    s >> meta.transferOverwrite;
    s >> meta.allowedClients;
    s >> meta.sourceBaseName;
    s >> meta.sourceType;
    s >> meta.targetRootPath;
    ///qDebug()<<"RECEIVED META"<<(quint32)meta.sourceType<<meta.allowedClients;

    QString myIp = getLocalIp();
    meta.allowed =meta.allowedClients.contains(myIp) || meta.allowedClients.contains("0.0.0.0");

    if(!meta.allowed) {
        log(QString("RECEIVED META BUT; NOT IN LIST (%1) → IGNORING").arg(myIp));
        return;
    }
    QString tmpFileName=meta.targetTempPath+"/"+meta.sourceBaseName;
    if(meta.transferOverwrite && QFile::exists(tmpFileName)) QFile::remove(tmpFileName);
    log(QString("RECEIVED META: %1 (%2) overwrite=%3 allowed=%4 transferId=%5") .arg(meta.sourcePath) .arg(static_cast<int>(meta.targetLocation)) .arg(meta.transferOverwrite).arg(meta.allowed).arg(meta.transferId));
    transferMap[meta.transferId] = meta;
    service->normFileReceiver->setAllowed(meta.allowed);
    sendMetaReply(meta,sender);
}

void TransferManager::normFileReceived(QString filePath, quint64 transferId, QString ip)
{
    if(!transferMap.contains(transferId))
       {
           qDebug()
               << "UNKNOWN TRANSFER ID"
               << transferId;
           return;
       }

       FileMeta meta =transferMap[transferId];
       qDebug()<<"normFileReceived"<<filePath<<transferId;
       qDebug()<< "FILE META FOUND:"<< meta.sourceBaseName;
       qDebug()<< "META ID:"<< meta.transferId;
       qDebug()<< "server ip:"<< ip;
       doDoneProcess(transferId,QHostAddress(ip));
}

void TransferManager::receivedMetaReply(QDataStream &s, QHostAddress &sender)
{

    quint64 tid;
    s >> tid;
    if(!transferMap.contains(tid))
        return;
    FileMeta fileMeta =transferMap.value(tid);
    if(tid == fileMeta.transferId)
    {
        log(QString("RECEIVED META_REPLY: %1 id:%2 IP: %3").arg(fileMeta.sourceBaseName).arg(fileMeta.transferId).arg(sender.toString()));
        sendFileStart(tid,normalizeIp(sender));
    }
}

void TransferManager::sendFileStart(quint64 tid,QString ip)
{
    if(!transferMap.contains(tid))
        return;
    FileMeta fileMeta =transferMap.value(tid);

    log(QString("FILE STARTING SENDER FOR ID: %1 IP: %2").arg(fileMeta.transferId).arg(ip));
    quint64 discoveryTime=service->clients[ip].discoveryTime;
    ///qDebug()<<"DiscoveryTime: "<<discoveryTime;

    QFile *file = new QFile(fileMeta.sourcePath);
    qint64 fileSize=file->size();
    quint64 totalTraffic =fileSize * fileMeta.allowedClients.count();
    //quint64 maxsize= 100ULL * 1024 * 1024;

    constexpr quint64 MiB = 1024ULL * 1024ULL;
    constexpr quint64 maxSize = 50ULL * MiB;
   /* if(fileMeta.allowedClients.count()<= 2)
    {
        /// TCPSENDER
        log(QString("SEND WITH TCPSENDER: %1 -- %2").arg(totalTraffic).arg(maxSize));
        QHostAddress addr(ip);
        emit startFileSender(addr,fileMeta,TCP_PORT,"/tmp",discoveryTime);
    }
    else
    {
        if(totalTraffic >= maxSize){
            /// NORMSENDER
             if (!normSenderFileMap.contains(fileMeta.transferId))
            {*/
                 log(QString("SEND WITH NORMSENDER: %1 -- %2").arg(totalTraffic).arg(maxSize));

                normSenderFileMap.insert(fileMeta.transferId, true);

                service->normFileSender->sendFile(fileMeta,
                                                  NORM_MULTICAST_IP,
                                                  NORM_PORT,
                                                  discoveryTime);
     /*       }
        }
        else{
            /// TCPSENDER
            log(QString("SEND WITH TCPSENDER: %1 -- %2").arg(totalTraffic).arg(maxSize));
            QHostAddress addr(ip);
            emit startFileSender(addr,fileMeta,TCP_PORT,"/tmp",discoveryTime);
        }
    }*/
}

void TransferManager::receivedStopTransfer(QDataStream &s, QHostAddress &sender)
{

    quint64 tid;
    QStringList clientList;
    s >> tid;
     s >> clientList;
    if(!transferMap.contains(tid))
        return;
    if(!clientList.contains(getLocalIp()))
        return;
    FileMeta fileMeta =transferMap.value(tid);
    if(tid == fileMeta.transferId)
    {
        log(QString("RECEIVED STOP_TRANSFER:id:%1 IP: %2").arg(fileMeta.transferId).arg(sender.toString()));
        service->fileReceiver->stopTransfer(tid);
        service->normFileReceiver->stopTransfer(tid);
    }
}


void TransferManager::sendStopTransfer(quint64 transferId,QStringList clientList)
{
    //qDebug()<<"stopTransfer";
    //qDebug()<<"transferId"<<transferId;
    //qDebug()<<"clientList"<<clientList;
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)STOP_TRANSFER;
    stream << transferId;
    stream << clientList;
    sendSocket.writeDatagram(msg,QHostAddress(MULTICAST_IP),UDP_PORT);
    log("SEND STOP_TRANSFER: id:"+QString::number(transferId));
    // LOCAL STOP
    service->fileReceiver->stopTransfer(transferId);
    service->normFileReceiver->stopTransfer(transferId);
}

void TransferManager::receivedDone(QDataStream &s, QHostAddress &sender)
{
    quint64 tid;
    s >> tid;
    if(!transferMap.contains(tid))
        return;

    FileMeta fileMeta =transferMap.value(tid);

    if(tid == fileMeta.transferId)
    {
        QString ip = sender.toString();
        if(!service->clients.contains(ip))
        {
            Client client;
            client.ip=ip;
            client.completed=true;
            service->clients[ip]=client;
        }else{
            service->clients[ip].completed=true;
        }
        ///log("CLIENT DONE: " + ip);

        /*emit this->transferFinished(ip,"server",fileMeta.sourcePath,
                                    QString::number(fileMeta.transferId)
                                    );
                                    */
        log(QString("RECEIVED DONE FOR ID: %1 IP: %2").arg(fileMeta.transferId).arg(sender.toString()));
        //transferMap[tid].allowed=true;

    }
}

void TransferManager::receivedProgress(QDataStream &s, QHostAddress &sender)
{
    quint64 packetId;
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
   ///qDebug()<<"Gelen PROGRESS"<<c->transferId;
    //log("SEND PROGRESS9: id:"+QString::number(c->transferId));
    service->monitorServer->sendProgressChanged(c);
}

void TransferManager::receivedMonitorRoleStatus(QDataStream &s, QHostAddress &sender)
{
    quint64 tid;
    QString role;
    bool sendToSender;
    bool sendToReceiver;
    bool receiver;
    bool getToOther;
    bool root;
    s >> tid;
    s >> role;
    s >> sendToSender;
    s >> sendToReceiver;
    s >> receiver;
    s >> getToOther;
    s >> root;

   /* qDebug()<<tid
            << sender.toString()
            << role
            << sendToSender
            << sendToReceiver
            << receiver
            << getToOther
            << root;
*/
    log("RECEIVED MONITOR_STATUS");
    QProcess p;

         p.start( "/usr/bin/dds-conf",
                  {"set-role", role,
                  sendToSender ? "true" : "false",
                  sendToReceiver ? "true" : "false",
                  receiver ? "true" : "false",
                  getToOther? "true" : "false",
                  root? "true" : "false"});

         p.waitForFinished();

         if(p.exitCode() == 0)
         {
             qDebug() << "Başarılı: Kaydedildi.";
             QTimer::singleShot(500, [this]()
             {
             service->roleInfo=getRole();
             service->discoveryManager->sendDiscovery();
             });
         }
         else
         {
             qDebug() << "Hata: kaydedilmedi";
         }

}

void TransferManager::receivedDebStart(QDataStream &s, QHostAddress &sender)
{
    quint64 tid;
    s >> tid;
    if(!transferMap.contains(tid))
        return;

    FileMeta fileMeta =
        transferMap.value(tid);
    log(QString("RECEIVED DEB_START"));
    if(tid == fileMeta.transferId)
    {
        QString ip = sender.toString();
        emit debStart(ip);
    }
}

void TransferManager::receivedDebDone(QDataStream &s, QHostAddress &sender)
{
    quint64 tid;
    QString status;

    s >> tid >> status;
    if(!transferMap.contains(tid))
        return;

    FileMeta fileMeta =
        transferMap.value(tid);
    log(QString("RECEIVED DEB_DONE"));
    if(tid == fileMeta.transferId)
    {
        QString ip = sender.toString();
        emit debDone(ip, status);
    }
}

void TransferManager::receivedScriptStart(QDataStream &s, QHostAddress &sender)
{
    quint64 tid;
    s >> tid;
    if(!transferMap.contains(tid))
        return;

    FileMeta fileMeta =
        transferMap.value(tid);
    log(QString("RECEIVED SCRIPT_START"));
    if(tid == fileMeta.transferId)
    {
        QString ip = sender.toString();
        //clientProgress[ip] = percent;
        emit scriptStart(ip);
    }
}

void TransferManager::receivedScriptDone(QDataStream &s, QHostAddress &sender)
{
    quint64 tid;
    QString status;

    s >> tid >> status;
    if(!transferMap.contains(tid))
        return;

    FileMeta fileMeta =transferMap.value(tid);
    log(QString("RECEIVED SCRIPT_DONE"));
    // qDebug()<<"script"<<tid << transferId;
    if(tid == fileMeta.transferId)
    {
        QString ip = sender.toString();
        emit scriptDone(ip, status);
    }
}

void TransferManager::sendMeta(FileMeta fileMeta)
{
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)META;
    stream << fileMeta.transferId;
    stream << fileMeta.sourcePath;
    stream << fileMeta.targetTempPath;
    stream << fileMeta.targetDestinationPath;
    stream << fileMeta.targetLocation;
    stream << fileMeta.transferOverwrite;
    stream << fileMeta.allowedClients;
    stream << fileMeta.sourceBaseName;
    stream << fileMeta.sourceType;
    stream << fileMeta.targetRootPath;

    sendSocket.writeDatagram(msg,QHostAddress(MULTICAST_IP),UDP_PORT);
    transferMap[fileMeta.transferId]=fileMeta;
    log("SEND META: " + fileMeta.sourcePath+" id:"+QString::number(fileMeta.transferId));
    ///qDebug()<<"sourceType" <<(quint32)fileMeta.sourceType;
}

void TransferManager::sendDone(quint64 transferId,const QHostAddress &sender,QString comment)
{
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)DONE;
    stream << transferId;
    sendSocket.writeDatagram(msg, sender, UDP_PORT);
    log(QString("%1 SEND DONE FOR ID: %2 IP: %3").arg(comment).arg(transferId).arg(sender.toString()));


}

void TransferManager::receivedGetFile(QDataStream &s, QHostAddress &sender)
{
    qDebug()<<"-------------------------------";
    quint64 packetId;
    QString myrole;
    QString myfile; SourceType mySourceType;
    QStringList clientList;
    SourceLocation sourceLocation;
    TargetLocation targetLocation;
    s >> packetId;
    s >> myrole;
    s >> myfile;
    s >> mySourceType;
    s >> clientList;
    s >> sourceLocation;
    s >> targetLocation;
    if(!clientList.contains(getLocalIp())) return;
    log(QString("RECEIVED GET_FILE:"));
    /************************************/
    service->getFileDoProcess(myrole,myfile,mySourceType,QStringList(sender.toString()),sourceLocation,targetLocation);
    /************************************/
}

void TransferManager::sendDoDoneProcess(quint64 transferId,const QHostAddress &sender)
{
    if(!transferMap.contains(transferId))
        return;
    if(!transferMap[transferId].allowed)
        return;
    FileMeta fileMeta =transferMap.value(transferId);
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)DO_DONE_PROCESS;
    stream << fileMeta.transferId;
    sendSocket.writeDatagram(msg,sender,UDP_PORT);
    log("SEND DO_DONE_PROCESS: " + fileMeta.sourcePath+" id:"+QString::number(fileMeta.transferId));
}

void TransferManager::receivedDoDoneProcess(QDataStream &s, QHostAddress &sender)
{
    quint64 tid;
    s >> tid;

    if(!transferMap.contains(tid))
        return;

    if(!transferMap[tid].allowed)
        return;
    FileMeta fileMeta =transferMap.value(tid);
    log(QString("RECEIVED DO_DONE_PROCESS: id: %1 IP: %2").arg(fileMeta.transferId).arg(sender.toString()));
}

void TransferManager::sendDebInstallStart(quint64 tid, const QHostAddress &sender)
{
      log("debInstallStart");
    QUdpSocket sendSocket;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)DEB_START;
    stream << tid;

    sendSocket.writeDatagram(msg, sender, UDP_PORT);
}

void TransferManager::sendDebInstallDone(QString status, const quint64 tid, const QHostAddress &sender)
{
    log("debInstallDone");
    QUdpSocket sendSocket;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)DEB_DONE;
    stream << tid;
    stream << status;

    sendSocket.writeDatagram(msg, sender, UDP_PORT);
}

void TransferManager::sendScriptInstallStart(quint64 tid, const QHostAddress &sender)
{
    log("scriptInstallStart");
    QUdpSocket sendSocket;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)SCRIPT_START;
    stream << tid;

    sendSocket.writeDatagram(msg, sender, UDP_PORT);
}

void TransferManager::sendScriptInstallDone(QString status, quint64 tid, const QHostAddress &sender)
{
    log("scriptInstallDone");
    QUdpSocket sendSocket;

    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);

    stream << (quint32)SCRIPT_DONE;
    stream << tid;
    stream << status;

    sendSocket.writeDatagram(msg, sender, UDP_PORT);
}

void TransferManager::setPermissionsRecursive(const QString &path, uid_t uid, gid_t gid)
{
    QFileInfo info(path);

    // Ana dosya/dizin
    ::chown(path.toUtf8().constData(), uid, gid);

    if(info.isDir())
        ::chmod(path.toUtf8().constData(), 0755);
    else
        ::chmod(path.toUtf8().constData(), 0755);

    // Alt içerikler
    QDirIterator it(path,
                    QDir::NoDotAndDotDot |
                        QDir::AllEntries,
                    QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        QString p = it.next();
        QFileInfo fi(p);

        ::chown(p.toUtf8().constData(), uid, gid);

        if(fi.isDir())
            ::chmod(p.toUtf8().constData(), 0755);
        else
            ::chmod(p.toUtf8().constData(), 0755);
    }
}
bool TransferManager::copyFile(const QString& src, const QString& dst, bool overwrite)
{
    ///qDebug()<<"copyFile"<<src<<dst;
    //QFileInfo info(src);
    //QString dst = dstDir + "/" + info.fileName();

    /* if(dstDir!="/tmp"){
    if(overwrite && QFile::exists(dst))
        QFile::remove(dst);
    }*/
    QFile::copy(src, dst);
    QFile::remove(src);
    return true;
}

bool TransferManager::copyDirectory(const QString &sourceDir,
                                    const QString &targetDir,
                                    bool overwrite)
{
    QDir src(sourceDir);

    if(!src.exists())
        return false;

    QDir target(targetDir);

    if(!target.exists())
    {
        if(!QDir().mkpath(targetDir))
            return false;
    }

    QFileInfoList entries = src.entryInfoList(
        QDir::NoDotAndDotDot |
        QDir::Files |
        QDir::Dirs);

    for(const QFileInfo &entry : entries)
    {
        QString srcPath = entry.absoluteFilePath();
        QString dstPath = targetDir + "/" + entry.fileName();

        if(entry.isDir())
        {
            if(!copyDirectory(srcPath, dstPath, overwrite))
                return false;
        }
        else
        {
            if(overwrite && QFile::exists(dstPath))
                QFile::remove(dstPath);

            if(!QFile::copy(srcPath, dstPath))
                return false;
        }
    }

    // KOPYA BİTTİKTEN SONRA SİLİNİYO
    return QDir(sourceDir).removeRecursively();
}




void TransferManager::onReceiveFinished(quint64 transferId, QString senderIp,QString filename)
{
    log(QString("FILE RECEIVED FOR ID: %1 IP: %2").arg(transferId).arg(senderIp));
    sendDone(transferId,QHostAddress(senderIp),"FileReceiver->");
    doDoneProcess(transferId,QHostAddress(senderIp));
}

void TransferManager::onSendFinished(quint64 transferId, QString senderIp,QString filename)
{
    log(QString("FILE SENT FOR ID: %1 IP: %2").arg(transferId).arg(senderIp));

}

void TransferManager::sendProgressChanged(Client *c)
{
    QUdpSocket sendSocket;
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << (quint32)PROGRESS;
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
    sendSocket.writeDatagram(msg,QHostAddress(c->serverIp),UDP_PORT);
    log("SEND PROGRESS: id:"+QString::number(c->transferId));
}



void TransferManager::doDoneProcess(quint64 transferId, const QHostAddress &sender)
{
    if(!transferMap.contains(transferId))
        return;

    FileMeta fileMeta = transferMap.value(transferId);

    sendDoDoneProcess(transferId, sender);

    log(QString("FILE DONE PROCESS FOR ID: %1").arg(transferId));

    UserPrivilegeHelper helper;
    SessionInfo userInfo = helper.getActiveSessionInfo();


    QString finalSourcePath =fileMeta.targetTempPath + "/" +
            QString::number(transferId) + "_" +fileMeta.sourceBaseName;
    QString extractPath;
    QString finalyDestFiletPath;
    QString finalDstPath =
            resolveTargetPath(fileMeta.targetLocation,
                              fileMeta.targetDestinationPath);

    if(fileMeta.sourceType!=SourceType::File)
    {


    QDir().mkpath(finalDstPath);

    extractPath = finalDstPath;

    log("ARCHIVE EXTRACT START: " + finalSourcePath);

    QProcess tar;
    tar.start("tar",
              {
                  "-xf",
                  finalSourcePath,
                  "-C",
                  extractPath
              });

    tar.waitForFinished(-1);

    if(tar.exitCode() != 0)
    {
        log("TAR EXTRACT FAILED");
        qDebug() << tar.readAllStandardError();
        return;
    }

    log("ARCHIVE EXTRACT DONE");

    QFile::remove(finalSourcePath);

    setPermissionsRecursive(extractPath,
                            userInfo.uid,
                            userInfo.gid);
    }
    else if(fileMeta.sourceType == SourceType::File)
    {
        qDebug() << "================ FILE MOVE START ================";
        qDebug() << "sourceType:" << (quint32)fileMeta.sourceType;
        qDebug() << "finalSourcePath:" << finalSourcePath;
        qDebug() << "fileMeta.sourceBaseName:" << fileMeta.sourceBaseName;
        qDebug() << "fileMeta.sourceBasePath:" << fileMeta.sourceBasePath;
        qDebug() << "fileMeta.targetRootPath:" << fileMeta.targetRootPath;
        qDebug() << "fileMeta.targetDestinationPath:" << fileMeta.targetDestinationPath;
        qDebug() << "finalDstPath:" << finalDstPath;
        setPermissionsRecursive(finalDstPath+"/DDS-Depo",userInfo.uid,userInfo.gid);
        QString postDestPath = finalDstPath + fileMeta.targetRootPath;
        qDebug() << "postDestPath:" << postDestPath;
        finalyDestFiletPath =postDestPath + "/" + fileMeta.sourceBaseName;
        qDebug() << "finalyDestFiletPath:" << finalyDestFiletPath;
        QFileInfo srcInfo(finalSourcePath);

        /*qDebug() << "SOURCE EXISTS:" << srcInfo.exists();
        qDebug() << "SOURCE SIZE:" << srcInfo.size();
        qDebug() << "SOURCE IS FILE:" << srcInfo.isFile();
        qDebug() << "SOURCE READABLE:" << srcInfo.isReadable();
        qDebug() << "SOURCE OWNER:" << srcInfo.owner();
        qDebug() << "SOURCE ABS PATH:" << srcInfo.absoluteFilePath();
        */
        QFileInfo dstInfo(finalyDestFiletPath);

        ///qDebug() << "DEST EXISTS BEFORE:" << dstInfo.exists();
        ///qDebug() << "DEST PATH:" << dstInfo.absoluteFilePath();

        // Dizin oluştur
        QFileInfo fi(finalyDestFiletPath);
        bool mkpathResult =QDir().mkpath(fi.absolutePath());
        ///qDebug() << "MKPATH RESULT:" << mkpathResult;
        ///qDebug() << "MKPATH ABS PATH:" << fi.absolutePath();
        QFileInfo dirInfo(fi.absolutePath());
        ///qDebug() << "DIR EXISTS:" << dirInfo.exists();
        ///qDebug() << "DIR WRITABLE:" << dirInfo.isWritable();
        ///qDebug() << "SET DIRECTORY PERMISSIONS";
        setPermissionsRecursive(postDestPath,userInfo.uid,userInfo.gid);
        bool removeOld =QFile::remove(finalyDestFiletPath);
        ///qDebug() << "REMOVE OLD FILE RESULT:" << removeOld;
        ///qDebug() << "TRYING RENAME";
        bool renameResult =QFile::rename(finalSourcePath,finalyDestFiletPath);
        ///qDebug() << "RENAME RESULT:" << renameResult;

        if(!renameResult)
        {
            QFile renameFile(finalSourcePath);
            ///qDebug() << "RENAME FAILED";
            ///qDebug() << "RENAME ERROR:" << renameFile.error();
            ///qDebug() << "RENAME ERROR STRING:"<< renameFile.errorString();
            ///qDebug() << "TRYING COPY";
            QFile copyFile(finalSourcePath);
            bool copyResult =copyFile.copy(finalyDestFiletPath);
            ///qDebug() << "COPY RESULT:" << copyResult;
            if(copyResult)
            {
                QFileInfo copiedInfo(finalyDestFiletPath);
                ///qDebug() << "COPIED FILE EXISTS:"<< copiedInfo.exists();
                ///qDebug() << "COPIED FILE SIZE:"<< copiedInfo.size();
                bool removeSource =QFile::remove(finalSourcePath);
                ///qDebug() << "REMOVE SOURCE RESULT:"<< removeSource;
            }
            else
            {
                ///qDebug() << "COPY FAILED";
                ///qDebug() << "COPY ERROR:"<< copyFile.error();
                ///qDebug() << "COPY ERROR STRING:"<< copyFile.errorString();
                QFileInfo copiedInfo(finalyDestFiletPath);
                ///qDebug() << "DEST EXISTS AFTER FAILED COPY:"<< copiedInfo.exists();
                ///qDebug() << "DEST SIZE AFTER FAILED COPY:"<< copiedInfo.size();
                ///qDebug() << "================ FILE MOVE FAILED ================";
                return;
            }
        }

        QFileInfo finalInfo(finalyDestFiletPath);
        ///qDebug() << "FINAL EXISTS:"<< finalInfo.exists();
        ///qDebug() << "FINAL SIZE:"<< finalInfo.size();
        ////qDebug() << "SET FILE PERMISSIONS";

        setPermissionsRecursive(finalyDestFiletPath, userInfo.uid,userInfo.gid);
        qDebug() << "================ FILE MOVE END ================";
    }
    /**************************************************
     * TARGET LOCATION ACTIONS
     **************************************************/

    switch(fileMeta.targetLocation)
    {
    case TargetLocation::FileCopyDesktop:
    case TargetLocation::FileCopyHome:
    case TargetLocation::FileCopyCustom:
    {
        // artık sadece extract + permission yeterli
        break;
    }

    case TargetLocation::DebInstall:
    {
        sendDebInstallStart(fileMeta.transferId, sender);

        QProcess esc;
        esc.start("systemd-escape", { finalyDestFiletPath });
        esc.waitForFinished();

        QString escapedPath =QString::fromUtf8(esc.readAllStandardOutput()).trimmed();

        QString serviceName =QString("dds-debinstaller@%1.service")
                .arg(escapedPath);

        QProcess p;
        p.start("systemctl", {"start", serviceName});
        p.waitForFinished();

        sendDebInstallDone(p.exitCode() == 0 ? "0" : "1",
                           fileMeta.transferId,sender);
        break;
    }

    case TargetLocation::ScriptExecute:
    {
        sendScriptInstallStart(fileMeta.transferId, sender);
        QProcess esc;
        esc.start("systemd-escape", { finalyDestFiletPath });
        esc.waitForFinished();

        QString escapedPath =QString::fromUtf8(esc.readAllStandardOutput()).trimmed();
        QString serviceName = QString("dds-scriptexec@%1.service").arg(escapedPath);

        QProcess p;
        p.start("systemctl", {"start", serviceName});
        p.waitForFinished();

        sendScriptInstallDone(p.exitCode() == 0 ? "0" : "1",
                              fileMeta.transferId,sender);
        break;
    }
    }
}


