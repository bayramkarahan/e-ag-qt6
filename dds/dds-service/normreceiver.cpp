#include "normreceiver.h"
#include "normreceiverworker.h"

NormReceiver::NormReceiver(DDSService *service,QObject *parent)
    : QObject(parent),service(service)
{
    log("NormReceiver START");
    worker = new NormReceiverWorker(service);
    worker->moveToThread(&thread);
    connect(&thread,&QThread::finished,worker,&QObject::deleteLater);
    connect(worker,&NormReceiverWorker::progress,this,&NormReceiver::progress);
    connect(worker,&NormReceiverWorker::fileReceived,this,&NormReceiver::fileReceived);
    connect(worker,&NormReceiverWorker::senderActive,this,&NormReceiver::senderActive);
    connect(worker,&NormReceiverWorker::senderInactive,this,&NormReceiver::senderInactive);
    connect(worker,&NormReceiverWorker::error,this,&NormReceiver::error);

    thread.start();
}

void NormReceiver::stopTransfer(quint64 transferId)
{
    QMetaObject::invokeMethod(worker,
                              "stopTransfer",
                              Qt::QueuedConnection,
                              Q_ARG(quint64, transferId));
}

NormReceiver::~NormReceiver()
{
    stop();

    thread.quit();

    thread.wait();
}

void NormReceiver::start(QString cacheDir,
                         QString multicastIp,
                         quint16 port)
{
    QMetaObject::invokeMethod(worker,
                              "startReceiver",
                              Qt::QueuedConnection,
                              Q_ARG(QString, cacheDir),
                              Q_ARG(QString, multicastIp),
                              Q_ARG(quint16, port));
}

void NormReceiver::stop()
{
    QMetaObject::invokeMethod(worker,
                              "stop",
                              Qt::QueuedConnection);
}
void NormReceiver::setAllowed(bool allowed)
{
    worker->allowed=allowed;
}
