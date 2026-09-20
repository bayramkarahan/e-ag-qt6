#include "rtppacketizer.h"

#include <QDataStream>
#include <QRandomGenerator>

RTPPacketizer::RTPPacketizer(QObject *parent)
    : QObject(parent)
{
    m_ssrc = QRandomGenerator::global()->generate();
}

QByteArray RTPPacketizer::createHeader(bool marker,
                                       quint8 payloadType,
                                       quint16 sequence,
                                       quint32 timestamp,
                                       quint32 ssrc)
{
    QByteArray header;
    header.resize(12);

    uchar *p = reinterpret_cast<uchar*>(header.data());

    // V=2 P=0 X=0 CC=0
    p[0] = 0x80;

    // M + PT
    p[1] = (marker ? 0x80 : 0x00) |
           (payloadType & 0x7F);

    // Sequence Number
    p[2] = sequence >> 8;
    p[3] = sequence & 0xFF;

    // Timestamp
    p[4] = timestamp >> 24;
    p[5] = timestamp >> 16;
    p[6] = timestamp >> 8;
    p[7] = timestamp;

    // SSRC
    p[8]  = ssrc >> 24;
    p[9]  = ssrc >> 16;
    p[10] = ssrc >> 8;
    p[11] = ssrc;

    return header;
}

static QByteArray removeStartCode(const QByteArray &nal)
{
    if(nal.size() >= 4 &&
       (uchar)nal[0] == 0 &&
       (uchar)nal[1] == 0 &&
       (uchar)nal[2] == 0 &&
       (uchar)nal[3] == 1)
    {
        return nal.mid(4);
    }

    if(nal.size() >= 3 &&
       (uchar)nal[0] == 0 &&
       (uchar)nal[1] == 0 &&
       (uchar)nal[2] == 1)
    {
        return nal.mid(3);
    }

    return nal;
}

void RTPPacketizer::packetize(const EncodedPacket &packet)
{

   /* qDebug()
        << "PACKETIZER IN"
        << "pts =" << packet.pts
        << "size =" << packet.data.size();
*/
    //
      // OPUS
      //
      if(packet.codec == CodecType::Opus)
      {
          RTPPacket rtp;

          rtp.sequence    = m_sequence++;
          rtp.timestamp = static_cast<quint32>(
              (packet.pts * 48000ULL) / 1000000ULL);
          rtp.ssrc        = m_ssrc;
          rtp.payloadType = 97;    // Dynamic payload type
          rtp.marker      = true;
          rtp.pts=packet.pts;
          QByteArray header =
                  createHeader(rtp.marker,
                               rtp.payloadType,
                               rtp.sequence,
                               rtp.timestamp,
                               rtp.ssrc);

          rtp.data.reserve(header.size() + packet.data.size());

          rtp.data.append(header);
          rtp.data.append(packet.data);
         /* qDebug()
              << "Packet PTS =" << packet.pts
              << "RTP TS =" << rtp.timestamp;
*/

          emit packetReady(rtp);
          return;
      }

      //
      // H264
      //

    if(packet.codec != CodecType::H264)
        return;

   /* qDebug().noquote()
        << "Size =" << packet.data.size()
        << "\n"
        << packet.data.left(120).toHex(' ');*/

    constexpr int RTP_HEADER_SIZE = 12;
    constexpr int RTP_MTU         = 1400;
    constexpr int FU_HEADER_SIZE  = 2;
    constexpr int MAX_PAYLOAD     = RTP_MTU - RTP_HEADER_SIZE - FU_HEADER_SIZE;

    QByteArray nal = removeStartCode(packet.data);
   /* qDebug().noquote()
        << nal.left(32).toHex(' ');*/

    int type = nal[0] & 0x1F;

   /* qDebug()
        << "NAL TYPE"
        << type
        << "SIZE"
        << nal.size()
        << "endOfFrame"
        << packet.endOfFrame;*/

    if(nal.isEmpty())
        return;

    const uchar *payload =
            reinterpret_cast<const uchar*>(nal.constData());

    int nalSize = nal.size();

    quint32 timestamp =
            static_cast<quint32>(
                (packet.pts * 90000ULL) / 1000000ULL);

    uchar nalHeader = payload[0];
    /*qDebug()
        << "NAL TYPE"
        << (nalHeader & 0x1F)
        << "size"
        << nalSize
        << "endOfFrame"
        << packet.endOfFrame;*/
    uchar f       = nalHeader & 0x80;
    uchar nri     = nalHeader & 0x60;
    uchar nalType = nalHeader & 0x1F;

    //
    // SINGLE NAL
    //
    if(nalSize <= (RTP_MTU - RTP_HEADER_SIZE))
    {
        RTPPacket rtp;

        rtp.sequence    = m_sequence++;
        rtp.timestamp   = timestamp;
        rtp.ssrc        = m_ssrc;
        rtp.payloadType = 96;
        rtp.marker = packet.endOfFrame;
        rtp.pts = packet.pts;
        QByteArray header =
                createHeader(rtp.marker,
                             rtp.payloadType,
                             rtp.sequence,
                             rtp.timestamp,
                             rtp.ssrc);

        rtp.data.reserve(header.size() + nalSize);

        rtp.data.append(header);

        // Start code'suz NAL gönderiyoruz.
        rtp.data.append(nal);
       /* qDebug()
            << "PACKETIZER OUT"
            << "pts =" << rtp.pts
            << "timestamp =" << rtp.timestamp
            << "seq =" << rtp.sequence
            << "marker =" << rtp.marker;*/
        emit packetReady(rtp);

        return;
    }

    //
    // FU-A
    //
    int offset = 1;

    while(offset < nalSize)
    {
        int payloadSize =
                qMin(MAX_PAYLOAD,
                     nalSize - offset);

        bool start = (offset == 1);
        bool end   = ((offset + payloadSize) >= nalSize);

        RTPPacket rtp;

        rtp.sequence    = m_sequence++;
        rtp.timestamp   = timestamp;
        rtp.ssrc        = m_ssrc;
        rtp.payloadType = 96;
        rtp.marker = end && packet.endOfFrame;
        rtp.pts = packet.pts;
        QByteArray header =
                createHeader(rtp.marker,
                             rtp.payloadType,
                             rtp.sequence,
                             rtp.timestamp,
                             rtp.ssrc);

        uchar fuIndicator = f | nri | 28;

        uchar fuHeader = nalType;

        if(start)
            fuHeader |= 0x80;

        if(end)
            fuHeader |= 0x40;

        rtp.data.reserve(header.size() + 2 + payloadSize);

        rtp.data.append(header);
        rtp.data.append(static_cast<char>(fuIndicator));
        rtp.data.append(static_cast<char>(fuHeader));

        rtp.data.append(
                    reinterpret_cast<const char*>(payload + offset),
                    payloadSize);
        /*qDebug()
            << "PACKETIZER OUT"
            << "pts =" << rtp.pts
            << "timestamp =" << rtp.timestamp
            << "seq =" << rtp.sequence
            << "marker =" << rtp.marker;*/

        emit packetReady(rtp);

        offset += payloadSize;
    }
}
