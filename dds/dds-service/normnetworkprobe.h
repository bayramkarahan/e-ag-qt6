#pragma once

#include <QObject>
#include <normApi.h>

class NormNetworkProbe : public QObject
{
    Q_OBJECT

public:
    explicit NormNetworkProbe(QObject *parent = nullptr);

    void update(NormSessionHandle session);

    double getRttMs() const;

signals:
    void rttUpdated(double rttMs);

private:
    double rttMs = 0.0;
};