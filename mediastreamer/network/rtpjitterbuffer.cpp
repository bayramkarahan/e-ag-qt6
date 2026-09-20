#include "rtpjitterbuffer.h"

#include <QDebug>

RTPJitterBuffer::RTPJitterBuffer(QObject *parent)
    : QObject(parent)
{
}

void RTPJitterBuffer::clear()
{
    m_buffer.clear();
    m_initialized = false;
}

void RTPJitterBuffer::processPacket(const RTPPacket &packet)
{
    if (!m_initialized)
    {
        m_initialized = true;
        m_expectedSequence = packet.sequence;
    }

    // Eski paket
    if ((quint16)(packet.sequence - m_expectedSequence) > 32768)
        return;

    m_buffer.insert(packet.sequence, packet);

    flush();
}

void RTPJitterBuffer::flush()
{
    while (m_buffer.contains(m_expectedSequence))
    {
        emit packetReady(m_buffer.take(m_expectedSequence));
        ++m_expectedSequence;
    }

    //
    // Bekleme sınırı aşıldıysa eksik paketi düşür.
    //
    while (m_buffer.size() > MAX_BUFFER_PACKETS)
    {
        qWarning() << "JitterBuffer drop packet" << m_expectedSequence;

        ++m_expectedSequence;

        while (m_buffer.contains(m_expectedSequence))
        {
            emit packetReady(m_buffer.take(m_expectedSequence));
            ++m_expectedSequence;
        }
    }
}
