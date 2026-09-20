#include "audiosender.h"

AudioSender::AudioSender(QObject *parent)
    : QObject(parent)
{
    connect(&m_encoder,
            &AudioOpusEncoder::packetReady,
            &m_packetizer,
            &RTPPacketizer::packetize,
            Qt::DirectConnection);

    connect(&m_packetizer,
            &RTPPacketizer::packetReady,
            &m_sender,
            &RtpSender::sendPacket,
            Qt::DirectConnection);
}

bool AudioSender::open(int sampleRate,
                       int channels,
                       int bitrate,
                       const QHostAddress &group,
                       quint16 port)
{
    if(!m_encoder.open(sampleRate,
                       channels,
                       bitrate))
    {
        return false;
    }

    if(!m_sender.open(group,
                      port))
    {
        m_encoder.close();
        return false;
    }



    return true;
}

void AudioSender::close()
{
    m_sender.close();
    m_encoder.close();
}

void AudioSender::onFrame(const AudioFrame &frame)
{
    /*qDebug()
        << "AUDIO SENDER"
        << frame.sampleRate
        << frame.channels
        << frame.data.size()
        << frame.pts;*/
    qint64 now = m_masterTime->nowUs();

        /*qDebug()
            << "AUDIO SENDER"
            << "pts =" << frame.pts
            << "delay =" << (now - frame.pts)
            << "thread =" << QThread::currentThread();*/
    m_encoder.encode(frame);
}

void AudioSender::addClient(const QHostAddress &addr,
                            quint16 port)
{
      m_sender.addDestination(addr, port);
}

void AudioSender::removeClient(const QHostAddress &addr,
                               quint16 port)
{
      m_sender.removeDestination(addr, port);
}
void AudioSender::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
    m_sender.setMasterTime(masterTime);
}

void AudioSender::clearClients()
{
       m_sender.clearDestinations();
}
