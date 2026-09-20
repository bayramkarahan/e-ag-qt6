#include "normsender.h"
#include "normsenderworker.h"

NormSender::NormSender(QObject *parent)
    : QObject(parent)
{
        log("NormSender START");
    worker = new NormSenderWorker;

    worker->moveToThread(&thread);

    connect(&thread,
            &QThread::finished,
            worker,
            &QObject::deleteLater);


    connect(worker,
            &NormSenderWorker::completed,
            this,
            &NormSender::completed);

    connect(worker,
            &NormSenderWorker::error,
            this,
            &NormSender::error);

    thread.start();
}

NormSender::~NormSender()
{
    stop();

    thread.quit();

    thread.wait();
}

void NormSender::sendFile(FileMeta fm,
                          QString multicastIp,
                          quint16 port,quint64 discoveryTime)
{
    QMetaObject::invokeMethod(worker,
                              "send",
                              Qt::QueuedConnection,
                              Q_ARG(FileMeta, fm),
                              Q_ARG(QString, multicastIp),
                              Q_ARG(quint16, port),
                              Q_ARG(quint64 ,discoveryTime));
}

void NormSender::stop()
{
    QMetaObject::invokeMethod(worker,
                              "stop",
                              Qt::QueuedConnection);
}
