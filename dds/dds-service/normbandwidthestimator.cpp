#include "normbandwidthestimator.h"

NormBandwidthEstimator::NormBandwidthEstimator(QObject *parent)
    : QObject(parent)
{
}

void NormBandwidthEstimator::startTest(qint64 bytes)
{
    totalBytes = bytes;
    timer.start();
}

void NormBandwidthEstimator::stopTest()
{
    qint64 ms = timer.elapsed();

    if(ms <= 0) return;

    double seconds = ms / 1000.0;

    double mbpsCalc =
        (totalBytes * 8.0) / (seconds * 1e6);

    mbps = mbpsCalc;

    emit bandwidthUpdated(mbps);
}

double NormBandwidthEstimator::getMbps() const
{
    return mbps;
}