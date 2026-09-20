#pragma once

#include <QObject>
#include <QThread>
#include <QAtomicInt>
#include "type.h"
#include <normApi.h>

class NormSenderWorker;

class NormSender : public QObject
{
    Q_OBJECT

public:
    explicit NormSender(QObject *parent = nullptr);
    ~NormSender();

    void sendFile(FileMeta fm,
                  QString multicastIp = "224.1.2.3",
                  quint16 port = 6003, quint64 discoveryTime=7);

    void stop();

signals:


    void completed();

    void error(QString text);

private:
    QThread thread;

    NormSenderWorker *worker = nullptr;
};
