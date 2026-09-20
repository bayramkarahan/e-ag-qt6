#include "filereceiver.h"

#include <QDir>

FileReceiver::FileReceiver(DDSService *service, QObject *parent)
    : QTcpServer(parent),service(service)
{
    log("FileReceiver STARTED");
}

void FileReceiver::stopTransfer(quint64 transferId)
{
    for(auto it = sessions.begin(); it != sessions.end(); ++it)
    {
        ReceiverSession *session = it.value();

        if(session->fileMeta.transferId != transferId)
            continue;

        if(session->file.isOpen())
            session->file.close();

        QString fileName = session->file.fileName();

        if(session->file.isOpen())
            session->file.close();

        if(!fileName.isEmpty())
            QFile::remove(fileName);

        it.key()->abort();

        it.key()->abort();   // socket
    }
}

void FileReceiver::incomingConnection(
        qintptr socketDescriptor)
{
    QTcpSocket *socket =new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    ReceiverSession *session =new ReceiverSession();

    sessions[socket] = session;

    connect(socket,&QTcpSocket::readyRead,this,&FileReceiver::readClient);
    connect(socket,&QTcpSocket::disconnected,this,&FileReceiver::clientDisconnected);
}

void FileReceiver::clientDisconnected()
{
    QTcpSocket *socket =
        qobject_cast<QTcpSocket*>(sender());

    ReceiverSession *session =
        sessions.take(socket);

    if(!session)
        return;

    if(session->file.isOpen())
        session->file.close();

    delete session;

    socket->deleteLater();
}

void FileReceiver::readClient()
{


    QTcpSocket *socket =qobject_cast<QTcpSocket*>(sender());

    ReceiverSession *session =sessions.value(socket);

    if(!session)
        return;


    QByteArray incoming =socket->readAll();



    if(incoming.isEmpty())
        return;

    //
    // HEADER OKUNMADIYSA
    //
    if(!session->headerRead)
    {
        session->headerBuffer.append(incoming);
        int pos =session->headerBuffer.indexOf('\n');
        if(pos == -1) return;
        QByteArray line =session->headerBuffer.left(pos);
        QByteArray remaining =session->headerBuffer.mid(pos + 1);

        QStringList p =QString::fromUtf8(line).trimmed().split("|");

        if(p.size() < 4)
        {
            qWarning() << "Invalid header:" << line;

            socket->disconnectFromHost();
            return;
        }

        session->fileMeta.transferId =p[1].toULongLong();
        session->filename =p[2];
        session->totalSize =p[3].toLongLong();

        if(!transferMap.contains(session->fileMeta.transferId)){
            qWarning()<< "Transfer ID not found:"<< session->fileMeta.transferId;
            socket->disconnectFromHost();
            return;
        }

        session->fileMeta =transferMap.value(session->fileMeta.transferId);

        QString tempFileName =
                "/tmp/"
                + QString::number(
                    session->fileMeta.transferId)
                + "_"
                + session->filename;

        session->file.setFileName(tempFileName);

        if(!session->file.open(QIODevice::WriteOnly))
        {
            qWarning()<< "Cannot open file:"<< tempFileName;
            socket->disconnectFromHost();
            return;
        }
        session->headerRead = true;
        session->startTime =QDateTime::currentMSecsSinceEpoch();
        session->lastTime =session->startTime;
        log(QString("RECEIVED HEADER FOR ID: %1").arg(session->fileMeta.transferId));

        //
        // HEADER SONRASI GELEN İLK DOSYA VERİSİ
        //
        if(!remaining.isEmpty())
        {
            session->file.write(remaining);
            session->received +=remaining.size();
        }
        session->headerBuffer.clear();
    }
    else
    {
        session->file.write(incoming);
        session->received +=incoming.size();
    }

    //
    // İLERLEME HESABI
    //
    int percent =(double)session->received* 100.0/ session->totalSize;
    qint64 now =QDateTime::currentMSecsSinceEpoch();
    qint64 elapsedMs =now - session->startTime;
    double elapsedSec =elapsedMs / 1000.0;
    double speedMB = 0;

    if(elapsedSec > 0)
    {
        speedMB =(session->received/ (1024.0 * 1024.0))/ elapsedSec;
    }

    qint64 remainSec = 0;

    if(speedMB > 0)
    {
        double remainMB = (session->totalSize - session->received) / (1024.0 * 1024.0);
        remainSec =remainMB / speedMB;
    }

    int step =(percent / 10) * 10;

    if(step != session->lastProgress)
    {
        session->lastProgress = step;
        Client c;
        c.transferId =session->fileMeta.transferId;
        c.ip =getLocalIp();
        c.hostName =QHostInfo::localHostName();
        c.fileName =session->filename;
        c.totalSize =session->totalSize;
        c.completed =false;
        c.received =session->received;
        c.speedData =speedMB;
        c.elapsedSec =elapsedSec;
        c.remainSec =remainSec;
        c.percent =step;

        c.serverIp =socket->peerAddress().toString();
        c.role =service->roleInfo.role;
        c.rolesendToSender =service->roleInfo.sendToSender;
        c.rolesendToReceiver =service->roleInfo.sendToReceiver;
        c.rolereceiver =service->roleInfo.receiver;
        c.rolegetToOther =service->roleInfo.getToOther;
        c.packetId =QRandomGenerator::global()->generate64();
        service->transferManager->sendProgressChanged(&c);
        c.ip =normalizeIp(socket->peerAddress());
        service->monitorServer->sendProgressChanged(&c);
    }

    //
    // DOSYA TAMAMLANDI
    //
    if(session->received>= session->totalSize)
    {
        session->file.close();
        emit receiveFinished(
                    session->fileMeta.transferId,
                    socket->peerAddress()
                    .toString(),
                    session->filename);
    }
}
