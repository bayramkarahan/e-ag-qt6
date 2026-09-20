#include "h264depacketizer.h"

#include <QDebug>
static inline qint64 rtpToPts(quint32 ts)
{
    return (static_cast<qint64>(ts) * 1000000LL) / 90000LL;
}

H264Depacketizer::H264Depacketizer(QObject *parent)
    : QObject(parent)
{
}

void H264Depacketizer::processPacket(const RTPPacket &packet)
{
    if(packet.data.isEmpty())
        return;

    const uchar *data =
            reinterpret_cast<const uchar*>(packet.data.constData());

    int size = packet.data.size();

    if(size < 1)
        return;

    //
    // Paket kaybı kontrolü
    //
    if(m_fuStarted)
    {
        if(packet.sequence != quint16(m_lastSequence + 1))
        {
            qWarning()
                    << "Lost RTP packet"
                    << m_lastSequence
                    << "->"
                    << packet.sequence;

            m_fuStarted = false;
            m_fuBuffer.clear();
        }
    }

    m_lastSequence = packet.sequence;

    uchar nalType = data[0] & 0x1F;

    //
    // Single NAL

    qDebug()
        << "RX NAL"
        << nalType
        << "marker"
        << packet.marker
        << "seq"
        << packet.sequence
        << "size"
        << size;
    if(nalType >= 1 && nalType <= 23)
    {
        QByteArray nal;

        nal.reserve(size + 4);
        nal.append("\x00\x00\x00\x01",4);
        nal.append(packet.data);

        emit nalReady(nal,
                      rtpToPts(packet.timestamp),
                      packet.marker);

        return;
    }

    //
    // FU-A
    //
    if(nalType == 28)
    {
        if(size < 2)
            return;

        uchar fuIndicator = data[0];
        uchar fuHeader    = data[1];

        bool start = fuHeader & 0x80;
        bool end   = fuHeader & 0x40;

        uchar originalNal =
                (fuIndicator & 0xE0) |
                (fuHeader & 0x1F);

        if(start)
        {
            m_fuStarted = true;

            m_fuBuffer.clear();
            m_fuBuffer.reserve(65536);

            m_fuBuffer.append("\x00\x00\x00\x01",4);
            m_fuBuffer.append(char(originalNal));
            m_fuBuffer.append(
                        reinterpret_cast<const char*>(data + 2),
                        size - 2);

            return;
        }

        if(!m_fuStarted)
            return;

        m_fuBuffer.append(
                    reinterpret_cast<const char*>(data + 2),
                    size - 2);

        if(end)
        {
            qDebug()
                << "FU END"
                << "size"
                << m_fuBuffer.size()
                << "pts"
                << rtpToPts(packet.timestamp)
                << "marker"
                << packet.marker;
            emit nalReady(m_fuBuffer,
                          rtpToPts(packet.timestamp),
                          packet.marker);

            m_fuBuffer.clear();
            m_fuStarted = false;
        }

        return;
    }

    qWarning() << "Unsupported NAL type:" << nalType;
}
