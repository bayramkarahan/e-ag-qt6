#include "normnetworkprobe.h"

NormNetworkProbe::NormNetworkProbe(QObject *parent)
    : QObject(parent)
{
}

void NormNetworkProbe::update(NormSessionHandle session)
{
    double grtt =NormGetGrttEstimate(session); // seconds

    rttMs = grtt * 1000.0;

    emit rttUpdated(rttMs);
}

double NormNetworkProbe::getRttMs() const
{
    return rttMs;
}