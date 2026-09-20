#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QHash>
#include<QDateTime>
#include<type.h>
#include<ddsservice.h>
#include<transfermanager.h>
class FileReceiver : public QTcpServer
{
    Q_OBJECT

public:
    explicit FileReceiver(DDSService *service, QObject *parent=nullptr);
    void stopTransfer(quint64 transferId);
signals:

    /*void receiveProgress(quint64 transferId,QString senderIp,
                         QString filename,
                         qint64 received,
                         qint64 total,
                         int percent,
                         double speedMB,
                         qint64 elapsedSec,
                         qint64 remainSec);*/
    void receiveProgress(Client *c);
    void receiveFinished(quint64 transferId, QString senderIp,QString filename);

protected:

    void incomingConnection(
            qintptr socketDescriptor) override;
    void clientDisconnected();
private slots:

    void readClient();


private:

    QHash<QTcpSocket*,ReceiverSession*> sessions;
    DDSService *service;
};

#endif
