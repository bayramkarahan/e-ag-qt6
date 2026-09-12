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
void H264Depacketizer::processNal(const QByteArray &nal,
                                  qint64 pts,
                                  bool marker)
{


    if (nal.size() < 5)
        return;

    const uint8_t *p =
            reinterpret_cast<const uint8_t*>(nal.constData());

    // 00 00 00 01 sonrasındaki NAL header
    uint8_t nalType = p[4] & 0x1f;
  /*  qDebug()
        << "NAL TYPE"
        << nalType
        << "SIZE"
        << nal.size();*/

    switch (nalType)
    {
    case 7:
        m_sps = nal;
        return;

    case 8:
        m_pps = nal;
        return;

    case 5:
    {
        if (!m_sps.isEmpty())
            emit nalReady(m_sps, pts, false);

        if (!m_pps.isEmpty())
            emit nalReady(m_pps, pts, false);

        m_started = true;

        emit nalReady(nal, pts, marker);
        return;
    }

    case 1:
    {
        if (!m_started)
            return;

        emit nalReady(nal, pts, marker);
        return;
    }

    default:
        return;
    }

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

   /* qDebug()
        << "RX NAL"
        << nalType
        << "marker"
        << packet.marker
        << "seq"
        << packet.sequence
        << "size"
        << size;*/
    if(nalType >= 1 && nalType <= 23)
    {
        QByteArray nal;

        nal.reserve(size + 4);
        nal.append("\x00\x00\x00\x01",4);
        nal.append(packet.data);

      /*  emit nalReady(nal,
                      rtpToPts(packet.timestamp),
                      packet.marker);*/
        processNal(nal,
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
            /*
            emit nalReady(m_fuBuffer,
                          rtpToPts(packet.timestamp),
                          packet.marker);*/
            processNal(m_fuBuffer,
                       rtpToPts(packet.timestamp),
                       packet.marker);
            m_fuBuffer.clear();
            m_fuStarted = false;
        }

        return;
    }

    qWarning() << "Unsupported NAL type:" << nalType;
}


void H264Depacketizer::processAnnexB(const QByteArray &data,
                                     qint64 pts)
{
    const uint8_t *buf =
            reinterpret_cast<const uint8_t*>(data.constData());

    int size = data.size();

    int pos = 0;

    while (pos + 4 < size)
    {
        int start = -1;
        int prefix = 0;

        //
        // Start Code ara
        //
        for (int i = pos; i + 3 < size; ++i)
        {
            if (buf[i] == 0 &&
                buf[i+1] == 0)
            {
                if (buf[i+2] == 1)
                {
                    start = i;
                    prefix = 3;
                    break;
                }

                if (i + 3 < size &&
                    buf[i+2] == 0 &&
                    buf[i+3] == 1)
                {
                    start = i;
                    prefix = 4;
                    break;
                }
            }
        }

        if (start < 0)
            break;

        //
        // Sonraki Start Code'u bul
        //
        int end = size;

        for (int i = start + prefix; i + 3 < size; ++i)
        {
            if (buf[i] == 0 &&
                buf[i+1] == 0)
            {
                if (buf[i+2] == 1)
                {
                    end = i;
                    break;
                }

                if (i + 3 < size &&
                    buf[i+2] == 0 &&
                    buf[i+3] == 1)
                {
                    end = i;
                    break;
                }
            }
        }

        QByteArray nal(
                    reinterpret_cast<const char*>(buf + start),
                    end - start);

        processNal(nal,
                   pts,
                   true);

        pos = end;
    }
}
