#include "rtpworker.h"

RtpWorker::RtpWorker(QObject *parent)
    : QObject(parent)
{
}
RtpWorker::~RtpWorker()
{
    stop();
}
void RtpWorker::setSettings(const StreamSettings &settings)
{
        m_settings = settings;
        qDebug()<<"RtspWorker::setSettings"<<
                  m_settings.video.width<<
                  m_settings.video.height<<
                  m_settings.video.fps;
}

bool RtpWorker::parseRtpPacket(const QByteArray &datagram,
                               RTPPacket &packet)
{
    if (datagram.size() < 12)
        return false;

    const uchar *p =
        reinterpret_cast<const uchar *>(datagram.constData());

    quint8 version = (p[0] >> 6) & 0x03;

    if (version != 2)
        return false;

    bool padding = p[0] & 0x20;
    bool extension = p[0] & 0x10;

    int csrcCount = p[0] & 0x0F;

    packet.marker = p[1] & 0x80;

    packet.payloadType = p[1] & 0x7F;

    packet.sequence =
        (quint16(p[2]) << 8) |
         quint16(p[3]);

    packet.timestamp =
        (quint32(p[4]) << 24) |
        (quint32(p[5]) << 16) |
        (quint32(p[6]) << 8) |
         quint32(p[7]);

    packet.ssrc =
        (quint32(p[8]) << 24) |
        (quint32(p[9]) << 16) |
        (quint32(p[10]) << 8) |
         quint32(p[11]);

    int headerSize = 12 + csrcCount * 4;

    if (datagram.size() < headerSize)
        return false;

    if (extension)
    {
        if (datagram.size() < headerSize + 4)
            return false;

        const uchar *ext =
            reinterpret_cast<const uchar *>(
                datagram.constData() + headerSize);

        quint16 extensionLength =
            (quint16(ext[2]) << 8) |
             quint16(ext[3]);

        headerSize += 4 + extensionLength * 4;

        if (datagram.size() < headerSize)
            return false;
    }

    int payloadSize = datagram.size() - headerSize;

    if (payloadSize <= 0)
        return false;

    packet.data =
        datagram.mid(headerSize, payloadSize);

    if (padding)
    {
        if (packet.data.isEmpty())
            return false;
/*
        quint8 paddingSize =
            static_cast<quint8>(
                packet.data.constLast());
*/
        quint8 paddingSize =
            static_cast<quint8>(
                static_cast<unsigned char>(
                    packet.data.at(packet.data.size() - 1)));

        if (paddingSize == 0 ||
            paddingSize > packet.data.size())
        {
            return false;
        }

        packet.data.chop(paddingSize);
    }

    return !packet.data.isEmpty();
}

void RtpWorker::process()
{
    if (m_running)
        return;

    m_running = true;

    m_videoSocket = new QUdpSocket(this);
    m_audioSocket = new QUdpSocket(this);

    QHostAddress videoGroup = m_settings.videoAddress;
    QHostAddress audioGroup = m_settings.audioAddress;

    if (!openSocket(m_videoSocket,
                    videoGroup,
                    m_settings.videoPort))
    {
        emit errorOccurred(
            "Cannot open video multicast socket");

        m_running = false;
        return;
    }

    if (!openSocket(m_audioSocket,
                    audioGroup,
                    m_settings.audioPort))
    {
        emit errorOccurred(
            "Cannot open audio multicast socket");

        m_videoSocket->close();

        m_running = false;
        return;
    }

    connect(m_videoSocket,
            &QUdpSocket::readyRead,
            this,
            &RtpWorker::readVideoDatagrams);

    connect(m_audioSocket,
            &QUdpSocket::readyRead,
            this,
            &RtpWorker::readAudioDatagrams);

    qDebug() << "RTP multicast receiver started";
    qDebug() << "Video:"
             << videoGroup
             << m_settings.videoPort;

    qDebug() << "Audio:"
             << audioGroup
             << m_settings.audioPort;

    emit connected();
}

bool RtpWorker::openSocket(QUdpSocket *socket,
                           const QHostAddress &group,
                           quint16 port)
{
    if (!socket->bind(QHostAddress::AnyIPv4,
                      port,
                      QUdpSocket::ShareAddress |
                      QUdpSocket::ReuseAddressHint))
    {
        qWarning()
            << "RTP bind failed"
            << port
            << socket->errorString();

        return false;
    }

    if (!socket->joinMulticastGroup(group))
    {
        qWarning()
            << "Cannot join multicast group"
            << group
            << socket->errorString();

        return false;
    }

    qDebug()
        << "Joined multicast"
        << group
        << port;

    return true;
}

void RtpWorker::readVideoDatagrams()
{
    while (m_videoSocket->hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(
            int(m_videoSocket->pendingDatagramSize()));

        QHostAddress sender;
        quint16 senderPort = 0;

        m_videoSocket->readDatagram(
            datagram.data(),
            datagram.size(),
            &sender,
            &senderPort);

        RTPPacket packet;

        if (!parseRtpPacket(datagram, packet))
        {
            qWarning() << "Invalid RTP video packet";
            continue;
        }

        if (packet.payloadType != 96)
            continue;

        emit videoPacket(packet);
    }
}

void RtpWorker::readAudioDatagrams()
{
    while (m_audioSocket->hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(
            int(m_audioSocket->pendingDatagramSize()));

        QHostAddress sender;
        quint16 senderPort = 0;

        m_audioSocket->readDatagram(
            datagram.data(),
            datagram.size(),
            &sender,
            &senderPort);

        RTPPacket packet;

        if (!parseRtpPacket(datagram, packet))
        {
            qWarning() << "Invalid RTP audio packet";
            continue;
        }

        if (packet.payloadType != 97)
            continue;

        emit audioPacket(packet);
    }
}

void RtpWorker::stop()
{
    if (!m_running)
        return;

    m_running = false;

    if (m_videoSocket)
    {
        m_videoSocket->leaveMulticastGroup(
            m_settings.videoAddress);

        m_videoSocket->close();

        m_videoSocket->deleteLater();
        m_videoSocket = nullptr;
    }

    if (m_audioSocket)
    {
        m_audioSocket->leaveMulticastGroup(
            m_settings.audioAddress);

        m_audioSocket->close();

        m_audioSocket->deleteLater();
        m_audioSocket = nullptr;
    }

    emit disconnected();
}


