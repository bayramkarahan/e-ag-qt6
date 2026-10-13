#include "rtpsource.h"
#include "rtpworker.h"

RtpSource::RtpSource(QObject *parent)
    : QObject(parent)
{
    m_thread = new QThread(this);

    m_worker = new RtpWorker;

    m_worker->moveToThread(m_thread);

    connect(this,
            &RtpSource::startWorker,
            m_worker,
            &RtpWorker::process);

    connect(this,
            &RtpSource::stopWorker,
            m_worker,
            &RtpWorker::stop);

    connect(m_worker,
            &RtpWorker::connected,
            this,
            &RtpSource::connected);

    connect(m_worker,
            &RtpWorker::disconnected,
            this,
            &RtpSource::disconnected);

    connect(m_worker,
            &RtpWorker::errorOccurred,
            this,
            &RtpSource::errorOccurred);

    connect(m_worker,
            &RtpWorker::videoPacket,
            this,
            &RtpSource::videoPacket);

    connect(m_worker,
            &RtpWorker::audioPacket,
            this,
            &RtpSource::audioPacket);

    connect(m_thread,
            &QThread::finished,
            m_worker,
            &QObject::deleteLater);

    m_thread->start();
}

RtpSource::~RtpSource()
{
    stop();

    m_thread->quit();
    m_thread->wait();
}

bool RtpSource::open(const StreamSettings &settings)
{
    m_settings = settings;

    m_worker->setSettings(settings);

    return true;
}

void RtpSource::start()
{
    emit startWorker();
}

void RtpSource::stop()
{
    emit stopWorker();
}

void RtpSource::close()
{
    stop();
}
