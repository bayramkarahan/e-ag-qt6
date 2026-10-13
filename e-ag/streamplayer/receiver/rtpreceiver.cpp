#include "rtpreceiver.h"

#include <QDebug>

RTPReceiver::RTPReceiver(QObject *parent)
    : QObject(parent)
{
}

bool RTPReceiver::open(const QHostAddress &group,
                       quint16 port)
{
    if (!m_socket.bind(QHostAddress::AnyIPv4,
                       port,
                       QUdpSocket::ShareAddress |
                       QUdpSocket::ReuseAddressHint))
    {
        qCritical() << m_socket.errorString();
        return false;
    }

    if (!m_socket.joinMulticastGroup(group))
    {
        qCritical() << "joinMulticastGroup failed";
        return false;
    }

    connect(&m_socket,
            &QUdpSocket::readyRead,
            this,
            &RTPReceiver::readPendingDatagrams);

    qDebug() << "Receiver joined" << group << port;

    return true;
}

void RTPReceiver::close()
{
    m_socket.close();
}

void RTPReceiver::readPendingDatagrams()
{
   /// qDebug() << "VIDEO UDP ARRIVED";
    while (m_socket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_socket.pendingDatagramSize());

        m_socket.readDatagram(datagram.data(),
                              datagram.size());

        if (datagram.size() < 12)
            continue;

        const quint8 *p =
            reinterpret_cast<const quint8 *>(datagram.constData());

        const int version = p[0] >> 6;
        const int csrcCount = p[0] & 0x0F;
        const bool extension = p[0] & 0x10;

        if (version != 2)
            continue;

        int headerSize = 12 + csrcCount * 4;

        if (extension)
        {
            if (datagram.size() < headerSize + 4)
                continue;

            const quint8 *ext = p + headerSize;
            quint16 length =
                    (ext[2] << 8) | ext[3];

            headerSize += 4 + length * 4;
        }

        if (datagram.size() < headerSize)
            continue;

        RTPPacket packet;

        packet.marker      = (p[1] & 0x80) != 0;
        packet.payloadType = p[1] & 0x7F;
        packet.sequence    = (p[2] << 8) | p[3];

        packet.timestamp =
                (quint32(p[4]) << 24) |
                (quint32(p[5]) << 16) |
                (quint32(p[6]) << 8)  |
                 quint32(p[7]);

        packet.ssrc =
                (quint32(p[8])  << 24) |
                (quint32(p[9])  << 16) |
                (quint32(p[10]) << 8)  |
                 quint32(p[11]);

        packet.data = datagram.mid(headerSize);
        /*qDebug() << "Receiver port"
                 << m_socket.localPort()
                 << "size"
                 << datagram.size();*/
///qDebug() << "EMIT RTP";
        emit packetReceived(packet);
    }
}
