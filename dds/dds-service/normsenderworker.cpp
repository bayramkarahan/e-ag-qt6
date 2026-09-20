#include "normsenderworker.h"

#include <QFileInfo>
#include <QDateTime>
#include <QElapsedTimer>
#include <QDebug>

NormSenderWorker::NormSenderWorker(QObject *parent)
    : QObject(parent)
{
    running = false;
}

void NormSenderWorker::send(FileMeta fm,
                            QString multicastIp,
                            quint16 port,quint64 discoveryTime)
{
    QString filePath=fm.sourcePath;
    interfaceType=detectNetworkType();
    qDebug()<<"interfaceType:"<<interfaceType<<filePath;
    running = true;

    QFileInfo info(filePath);

    QByteArray ip   = multicastIp.toUtf8();
    QByteArray path = filePath.toUtf8();
    QString normName =
        QString("%1_%2")
            .arg(fm.transferId)
            .arg(fm.sourceBaseName);

    QByteArray name = normName.toUtf8();

    ///QByteArray name = info.fileName().toUtf8();

    instance = NormCreateInstance();

    if(instance == NORM_INSTANCE_INVALID)
    {
        emit error("NormCreateInstance failed");
        return;
    }

    session = NormCreateSession(instance,
                                ip.constData(),
                                port,
                                1);

    if(session == NORM_SESSION_INVALID)
    {
        emit error("NormCreateSession failed");
        return;
    }

    NormSetRxPortReuse(session, true);

    NormSetMulticastLoopback(session, false);

    NormSessionId sessionId =
        (NormSessionId)
        QDateTime::currentMSecsSinceEpoch();

        double currentRate = 60.0e+06;

    //if(interfaceType == "wifi")
        NormSetTxRate(session,currentRate);
        NormSetTxSocketBuffer(session,16*1024*1024);
        NormSetBackoffFactor(session,0.0);
        NormSetGroupSize(session,1);
        NormSetGrttEstimate(session,0.010);
        NormStartSender(session,
                        sessionId,
                       32*1024*1024,
                        1400,
                        256,
                        8);
    NormObjectHandle obj=NormFileEnqueue(session,
                    path.constData(),
                    name.constData(),
                    name.size());


    log("Sender started");
    while(running)
    {
        NormEvent event;

        if(!NormGetNextEvent(instance,
                             &event))
        {
            continue;
        }

        switch(event.type)
        {
        case NORM_TX_QUEUE_EMPTY:

            emit log("QUEUE EMPTY");

            break;

        case NORM_TX_FLUSH_COMPLETED:

            emit log("FLUSH COMPLETED");

            estimator.stopTest();

            emit completed();

            running = false;

            break;

        default:
            break;
        }

    }

    NormStopSender(session);

    NormDestroySession(session);

    NormDestroyInstance(instance);

    session  = NORM_SESSION_INVALID;
    instance = NORM_INSTANCE_INVALID;
}

void NormSenderWorker::stop()
{
    running = false;
}


QString NormSenderWorker::detectNetworkType()
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
                ///log("ACTIVE IFACE: " + activeIface);

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
