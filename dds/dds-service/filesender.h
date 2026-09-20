#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>
#include <QTcpSocket>
///#include <QList>
#include "type.h"
#include <QHash>
class FileSender : public QObject
{
    Q_OBJECT

public:
    explicit FileSender(QObject *parent = nullptr);

    void sendFile(QHostAddress &sender, FileMeta fileMeta, quint16 port, QString savePath, quint64 discoveryTime);

signals:

    void clientConnected(QString ip);

    void transferProgress(QString ip,
                          qint64 sent,
                          qint64 total,
                          int percent);

    void transferFinished(quint64 transferId, QString senderIp,QString filename);

    void transferError(QString ip,
                       QString error);

private slots:

    void socketConnected();
    void socketBytesWritten(qint64 bytes);
    void socketDisconnected();
    int calculateChunkSize(quint64 discoveryTime);
private:

    void sendNextChunk(TransferSession *session);

    ///QList<TransferSession*> sessions;


    QHash<QTcpSocket*, TransferSession*> sessions;
    //QByteArray sharedBuffer;

    //QString sharedFilename;

    //QString sharedSavePath;

    //int chunkSize = 512 * 1024;

};

#endif
