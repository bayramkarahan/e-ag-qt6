#include "rtspsource.h"
#include "rtspworker.h"

RtspSource::RtspSource(QObject *parent)
    : QObject(parent)
{
    m_thread = new QThread(this);

    m_worker = new RtspWorker;
    m_worker->moveToThread(m_thread);

    connect(this,
            &RtspSource::startWorker,
            m_worker,
            &RtspWorker::process);

    connect(this,
            &RtspSource::stopWorker,
            m_worker,
            &RtspWorker::stop);

    connect(m_worker,
            &RtspWorker::connected,
            this,
            &RtspSource::connected);

    connect(m_worker,
            &RtspWorker::disconnected,
            this,
            &RtspSource::disconnected);

    connect(m_worker,
            &RtspWorker::errorOccurred,
            this,
            &RtspSource::errorOccurred);

    connect(m_thread,
            &QThread::finished,
            m_worker,
            &QObject::deleteLater);

    connect(m_worker,
            &RtspWorker::videoPacket,
            this,
            &RtspSource::videoPacket);

    connect(m_worker,
            &RtspWorker::audioPacket,
            this,
            &RtspSource::audioPacket);

      m_thread->start();
}

RtspSource::~RtspSource()
{
    stop();

    m_thread->quit();
    m_thread->wait();
}

bool RtspSource::open(const QString &url, const StreamSettings &settings)
{
    m_url = url;
    m_settings = settings;
    m_worker->setSettings(m_settings);
    m_worker->setUrl(url);

    return true;
}

void RtspSource::close()
{
    stop();
}

void RtspSource::start()
{
    emit startWorker();
}

void RtspSource::stop()
{
    emit stopWorker();
}
