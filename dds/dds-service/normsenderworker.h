#pragma once

#include <QObject>
#include <QAtomicInt>

#include <normApi.h>
#include<normnetworkprobe.h>
#include<normbandwidthestimator.h>
//#include<normratecontroller.h>
#include<QNetworkInterface>
#include "type.h"
class NormSenderWorker : public QObject
{
    Q_OBJECT

public:
    explicit NormSenderWorker(QObject *parent = nullptr);
    QString interfaceType;
public slots:
    void send(FileMeta fm,
              QString multicastIp,
              quint16 port,quint64 discoveryTime);

    void stop();
    QString detectNetworkType();

signals:
    void completed();

    void error(QString text);

private:
    QAtomicInt running;

    NormInstanceHandle instance = NORM_INSTANCE_INVALID;
    NormSessionHandle session = NORM_SESSION_INVALID;
    NormNetworkProbe probe;
    //NormRateController controller;
    NormBandwidthEstimator estimator;
};
