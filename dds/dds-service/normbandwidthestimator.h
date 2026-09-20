#pragma once

#include <QObject>
#include <QElapsedTimer>

class NormBandwidthEstimator : public QObject
{
    Q_OBJECT

public:
    explicit NormBandwidthEstimator(QObject *parent = nullptr);

    void startTest(qint64 bytes);
    void stopTest();

    double getMbps() const;

signals:
    void bandwidthUpdated(double mbps);

private:
    QElapsedTimer timer;
    qint64 totalBytes = 0;

    double mbps = 0.0;
};