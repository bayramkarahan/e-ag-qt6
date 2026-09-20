#include "filesender.h"

#include <QFile>
#include <QFileInfo>

FileSender::FileSender(QObject *parent)
    : QObject(parent)
{
    log("FileSender START");
}

void FileSender::sendFile(QHostAddress &sender,FileMeta fileMeta,
                          quint16 port,
                          QString savePath,quint64 discoveryTime)
{
    int tempchunkSize=calculateChunkSize(discoveryTime);
    ///qDebug()<<"discoveryTime"<<discoveryTime<<"chunkSize"<<tempchunkSize;
    Q_UNUSED(savePath)
    QFileInfo info(fileMeta.sourcePath);
    QString ip=normalizeIp(sender);
    //for(QString ip : fileMeta.allowedClients)
   // {
        QFile *file = new QFile(fileMeta.sourcePath);

        if(!file->open(QIODevice::ReadOnly))
        {
            delete file;
            return;
            //continue;
        }

        QTcpSocket *socket = new QTcpSocket(this);

        TransferSession *session = new TransferSession();
        session->chunkSize=tempchunkSize;
        session->fileMeta = fileMeta;
        session->socket = socket;
        session->file = file;
        session->ip = ip;
        session->filename = info.fileName();
        session->totalSize = file->size();

        ///sessions.append(session);
        sessions.insert(socket, session);
        connect(socket,
                &QTcpSocket::connected,
                this,
                &FileSender::socketConnected);

        connect(socket,
                &QTcpSocket::bytesWritten,
                this,
                &FileSender::socketBytesWritten);

        connect(socket,
                &QTcpSocket::disconnected,
                this,
                &FileSender::socketDisconnected);

      /*  connect(socket,
                &QTcpSocket::errorOccurred,
                this,
                [](QAbstractSocket::SocketError e)
                {
                    qDebug() << "TCP ERROR" << e;
                });
*/
        connect(socket,
                &QTcpSocket::errorOccurred,
                this,
                [this, socket](QAbstractSocket::SocketError e)
        {
            qDebug() << e;

            TransferSession *session =sessions.take(socket);

            if(session)
            {
                if(session->file)
                {
                    session->file->close();
                    delete session->file;
                }

                socket->deleteLater();
                delete session;
            }
        });

        socket->connectToHost(ip, port);
   // }
}


void FileSender::socketConnected()
{
    QTcpSocket *socket =
        qobject_cast<QTcpSocket*>(sender());

    /*for(TransferSession *session : sessions)
   /// {
        if(session->socket != socket)
            continue;
*/
    TransferSession *session = sessions.value(socket);

    if(!session)
        return;

        QString header =
            QString("FILE|%1|%2|%3\n")
                .arg(session->fileMeta.transferId)
                .arg(session->filename)
                .arg(session->totalSize);

        socket->write(header.toUtf8());

        session->headerSent = true;

        sendNextChunk(session);

       /// break;
   /// }
}

void FileSender::socketBytesWritten(qint64)
{
    QTcpSocket *socket =
            qobject_cast<QTcpSocket*>(sender());

    TransferSession *session =
            sessions.value(socket);

    if(!session)
        return;

    if(session->completed)
    {
        if(socket->bytesToWrite() == 0)
        {
            emit transferFinished(
                        session->fileMeta.transferId,
                        session->ip,
                        session->fileMeta.sourceBaseName);

            socket->disconnectFromHost();
        }

        return;
    }

    sendNextChunk(session);
}

void FileSender::sendNextChunk(TransferSession *session)
{
    if(session->completed)
        return;

    QTcpSocket *socket = session->socket;

    while(socket->bytesToWrite() < session->chunkSize)
    {
        QByteArray chunk =
                session->file->read(session->chunkSize);

        if(chunk.isEmpty())
        {
            // Dosya okuma tamamlandı.
            // Ama TCP bufferında hala veri olabilir.
            session->completed = true;

            if(socket->bytesToWrite() == 0)
            {
                emit transferFinished(
                            session->fileMeta.transferId,
                            session->ip,
                            session->fileMeta.sourceBaseName);

                socket->disconnectFromHost();
            }

            return;
        }

        qint64 written = socket->write(chunk);

        if(written <= 0)
            return;

        session->offset += written;

        /*int percent =
                (double)session->offset * 100.0 /
                session->totalSize;

        int step = (percent / 10) * 10;

        if(step != session->lastProgress)
        {
            session->lastProgress = step;


            emit transferProgress(
                        session->ip,
                        session->offset,
                        session->totalSize,
                        step);

        }*/
    }
}

void FileSender::socketDisconnected()
{
    QTcpSocket *socket =
        qobject_cast<QTcpSocket*>(sender());
    TransferSession *session =sessions.take(socket);

    if(!session)
        return;

    if(session->file)
    {
        session->file->close();
        delete session->file;
    }

    socket->deleteLater();

    delete session;
   /* for(int i=0;i<sessions.size();++i)
    {
        TransferSession *session =
            sessions[i];

        if(session->socket != socket)
            continue;

        if(session->file)
        {
            session->file->close();
            delete session->file;
        }

        socket->deleteLater();

        sessions.removeAt(i);

        delete session;

        break;
    }*/
}

int FileSender::calculateChunkSize(quint64 discoveryTime)
{
    if(discoveryTime < 5)
    {
        // hızlı LAN
        return 1024 * 1024;   // 1 MB
    }
    else if(discoveryTime < 20)
    {
        return 512 * 1024;    // 512 KB
    }
    else if(discoveryTime < 50)
    {
        return 256 * 1024;    // 256 KB
    }
    else
    {
        return 64 * 1024;     // yavaş ağ
    }
}
