#include "normreceiverworker.h"

#include <QDir>

NormReceiverWorker::NormReceiverWorker(DDSService *service, QObject *parent)
    : QObject(parent),service(service)
{
    running = false;
}

void NormReceiverWorker::startReceiver(QString cacheDir,
                                       QString multicastIp,
                                       quint16 port)
{
    interfaceType=detectNetworkType();
    qDebug()<<"interfaceType:"<<interfaceType;

    running = true;

    QByteArray ip = multicastIp.toUtf8();

    QByteArray cache = cacheDir.toUtf8();

    instance = NormCreateInstance();

    if(instance == NORM_INSTANCE_INVALID)
    {
        emit error("NormCreateInstance failed");
        return;
    }

    session = NormCreateSession(instance,
                                ip.constData(),
                                port,
                                NORM_NODE_ANY);

    NormSetRxPortReuse(session, true);
    NormSetMulticastLoopback(session, false);
    ///NormSetRxLoss(session, 10.0);

    if(!NormSetCacheDirectory(instance,
                               cache.constData()))
    {
        emit error("NormSetCacheDirectory failed");
        return;
    }
    //if(interfaceType == "wifi")
    NormSetRxSocketBuffer(session,16*1024*1024);
    NormStartReceiver(session,32*1024*1024);

   log("Receiver started");

    while(running)
    {
        NormEvent event;

        ///if(!NormGetNextEvent(instance, &event))
        if(!NormGetNextEvent(instance,
                             &event,
                             true))
            continue;
         switch(event.type)
        {

        case NORM_REMOTE_SENDER_NEW:
        {
            log("REMOTE_SENDER_NEW");
            QString ip =getSenderIp(event.sender);
                if(!ip.isEmpty()){
                    senderIpMap.insert(event.sender, ip);
                    qDebug()<< "NORM SERVER IP:"<< ip;
                }
                break;
        }
        case NORM_REMOTE_SENDER_ACTIVE:
            log("REMOTE_SENDER_ACTIVE");
            emit senderActive();
            break;

        case NORM_REMOTE_SENDER_INACTIVE:
         {
            log("REMOTE_SENDER_INACTIVE");
            ///senderIpMap.remove(event.sender);
            emit senderInactive();
            break;
         }
        case NORM_RX_OBJECT_NEW:
        {
            NormProgressInfo info;
            info.startTime = QDateTime::currentMSecsSinceEpoch();
            progressMap.insert(event.object, info);

            log("RX_OBJECT_NEW");
             if(!allowed)
                {
                    log("REJECT OBJECT");

                    NormObjectCancel(event.object);

                    /**************************/
                    char path[2048];
                    if(NORM_OBJECT_FILE ==
                    NormObjectGetType(event.object))
                    {
                        NormFileGetName(event.object,
                                    path,
                                    2048);
                        //emit log(QString("remove TEMP FILE: %1").arg(path));
                        QFile::remove(path);
                    }
                    /***********************************/
                    break;
                }
         break;
         }

         case NORM_RX_OBJECT_INFO:
         {

             log("RX_OBJECT_INFO");

             char info[1024];

             unsigned short len =
                 NormObjectGetInfo(event.object,
                                   info,
                                   sizeof(info)-1);

             info[len] = 0;

             QString normName = QString::fromUtf8(info);

             int pos = normName.indexOf('_');

             if(pos > 0)
             {
                 quint64 transferId =
                     normName.left(pos).toULongLong();

                 objectTransferMap.insert(event.object,
                                          transferId);
                 transferObjectMap[transferId] = event.object;
                 qDebug()
                     << "TRANSFER ID REGISTERED:"
                     << transferId<<normName;
             }

             break;
         }


         case NORM_RX_OBJECT_UPDATED:
         {

             NormSize total =NormObjectGetSize(event.object);
             NormSize completed =total -NormObjectGetBytesPending(event.object);
             if(total <= 0)  break;
             NormProgressInfo &info =progressMap[event.object];
             int percent =(int)((double)completed * 100.0 /(double)total);
             int step =(percent / 10) * 10;
             // sadece %10 değişimlerinde gönder
             if(step != info.lastProgress){
                 info.lastProgress = step;
             qint64 now =QDateTime::currentMSecsSinceEpoch();
             double elapsedSec =(now - info.startTime) / 1000.0;
             double speedMB = 0;
             if(elapsedSec > 0){
                     speedMB =(completed /(1024.0 * 1024.0))/elapsedSec;
             }
             qint64 remainSec = 0;
             if(speedMB > 0){
                     double remainMB =(total - completed) /(1024.0 * 1024.0);
                     remainSec =remainMB / speedMB;
                 }
                 char fileInfo[1024];
                 unsigned short len =NormObjectGetInfo(event.object,fileInfo,1023);

                 fileInfo[len] = 0;
                 QString filename =QString::fromUtf8(fileInfo);
                 qDebug()<< "FILE:"<< filename<< "STEP:"<< step
                         << "SPEED:"<< speedMB<< "MB/s";
                 Client c;
                 // transferId map'ten alınacak
                 c.transferId =objectTransferMap.value(event.object);
                 c.ip =getLocalIp();
                 c.hostName =QHostInfo::localHostName();
                 c.fileName =filename;
                 c.totalSize =total;
                 c.completed = false;
                 c.received =completed;
                 c.speedData =speedMB;
                 c.elapsedSec =elapsedSec;
                 c.remainSec =remainSec;
                 c.percent =step;
                 // NORM sender IP
                 NormNodeHandle sender =NormObjectGetSender(event.object);
                 c.serverIp =senderIpMap.value(sender);
                 c.role =service->roleInfo.role;
                 c.rolesendToSender =service->roleInfo.sendToSender;
                 c.rolesendToReceiver =service->roleInfo.sendToReceiver;
                 c.rolereceiver =service->roleInfo.receiver;
                 c.rolegetToOther =service->roleInfo.getToOther;
                 c.packetId =QRandomGenerator::global()->generate64();
                 service->transferManager->sendProgressChanged(&c);
                 c.ip =normalizeIp(QHostAddress(c.ip));
                 service->monitorServer->sendProgressChanged(&c);
             }


             break;
         }
        case NORM_RX_OBJECT_COMPLETED:
        {
            log("RX_OBJECT_COMPLETED");

            char normTempPath[2048];

            if (NORM_OBJECT_FILE == NormObjectGetType(event.object))
            {

               NormNodeHandle sender =NormObjectGetSender(event.object);
               QString  ip =senderIpMap.value(sender);
               char tFName[1024];
               unsigned short len =NormObjectGetInfo(event.object,
                            tFName,sizeof(tFName)-1);

                tFName[len] = 0;
                QString tempFileName = QString::fromUtf8(tFName);
                NormFileGetName(event.object,normTempPath,sizeof(normTempPath));
                quint64 transferId = objectTransferMap.take(event.object);

                transferObjectMap.remove(transferId);
                progressMap.remove(event.object);

                int pos = tempFileName.indexOf('_');

                if (pos <= 0)
                {
                    log(QString("INVALID NORM FILE NAME: %1")
                            .arg(tempFileName));
                    break;
                }

                QString realFileName =tempFileName.mid(pos + 1);
                qDebug() << "OBJECT INFO =" << tempFileName;
                qDebug() << "Norm Temp FILE NAME   :" << normTempPath;
                qDebug() << "TRANSFER ID :" << transferId;
                qDebug() << "FILE NAME   :" << realFileName;
                QString finalPath="/tmp/"+tempFileName;
                if(QFile::rename(normTempPath,finalPath))
                {
                    qDebug() << "MOVE OK:" << finalPath;
                    emit fileReceived(realFileName,transferId,ip);
                }
                else
                {
                    qDebug() << "MOVE FAILED";
                }

            }

            break;
        }

         case NORM_RX_OBJECT_ABORTED:
         {
             log("RX_OBJECT_ABORTED");

             quint64 transferId = objectTransferMap.take(event.object);

             if(transferId != 0)
                 transferObjectMap.remove(transferId);

             progressMap.remove(event.object);

             if(NormObjectGetType(event.object) == NORM_OBJECT_FILE)
             {
                 char path[2048];

                 NormFileGetName(event.object,
                                 path,
                                 sizeof(path));

                 QFile::remove(path);

                 qDebug() << "TEMP FILE REMOVED:" << path;
             }

             qDebug() << "TRANSFER ABORTED:" << transferId;

             break;
         }

        default:
            break;
        }
    }

    NormStopReceiver(session);

    NormDestroySession(session);

    NormDestroyInstance(instance);

    session = NORM_SESSION_INVALID;

    instance = NORM_INSTANCE_INVALID;
}

void NormReceiverWorker::stop()
{
    running = false;
}


void NormReceiverWorker::stopTransfer(quint64 transferId)
{
    if(!transferObjectMap.contains(transferId))
    {
        qDebug() << "TRANSFER NOT FOUND:" << transferId;
        return;
    }

    NormObjectHandle object = transferObjectMap.value(transferId);

    qDebug() << "CANCEL TRANSFER:" << transferId;

    NormObjectCancel(object);
}


QString NormReceiverWorker::detectNetworkType()
{
    QString activeIface;

    for (const QNetworkInterface &iface : QNetworkInterface::allInterfaces())
    {
        //log(QString("%1 | %2")
        //   .arg(iface.name())
        // .arg(iface.humanReadableName()));

        // sadece çalışanlar
        if (!(iface.flags() & QNetworkInterface::IsUp) ||
            !(iface.flags() & QNetworkInterface::IsRunning))
            continue;

        // loopback skip
        if (iface.flags() & QNetworkInterface::IsLoopBack)
            continue;

        // IP var mı?
        for (const QNetworkAddressEntry &entry : iface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                activeIface = iface.name();
               /// log("ACTIVE IFACE: " + activeIface);

                // 🔥 Linux naming standard
                if (activeIface.startsWith("wl"))
                    return "wifi";

                if (activeIface.startsWith("en") || activeIface.startsWith("eth"))
                    return "ethernet";

                return "unknown";
            }
        }
    }

    return "unknown";
}

QString NormReceiverWorker::getSenderIp(NormNodeHandle sender)
{
    char tempip[64];

    unsigned int length = sizeof(tempip);

    if(!NormNodeGetAddress(sender,
                           tempip,
                           &length))
    {
        return "";
    }


    // IPv4 ise
    if(length == 4)
    {
        quint32 addr;

        memcpy(&addr,
               tempip,
               4);

        return QHostAddress(ntohl(addr))
                .toString();
    }


    return "";
}
