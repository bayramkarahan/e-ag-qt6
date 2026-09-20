#include "mastertime.h"

MasterTime::MasterTime(QObject *parent)
    : QObject(parent)
{
}

void MasterTime::start()
{
    m_timer.start();
}

void MasterTime::reset()
{
    m_timer.restart();
}

qint64 MasterTime::nowUs() const
{
    if(!m_timer.isValid())
        return 0;

    return m_timer.nsecsElapsed() / 1000;
}
