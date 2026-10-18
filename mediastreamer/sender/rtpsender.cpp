#include "rtpsender.h"

#include <QDebug>
#include <QAbstractSocket>

RtpSender::RtpSender(QObject *parent)
    : QObject(parent)
{
}

bool RtpSender::open(const QHostAddress &group,
                     quint16 port,
                     SenderMode sendermode,
                     int ttl)
{
    close();

    m_group = group;
    m_port = port;
    m_sendermode = sendermode;

    if (m_sendermode == SenderMode::Multicast)
    {
        return openMulticastInterfaces(ttl);
    }

    // Unicast
    m_socket.setSocketOption(
        QAbstractSocket::SendBufferSizeSocketOption,
        16 * 1024 * 1024);

    qDebug()
        << "Unicast send buffer ="
        << m_socket.socketOption(
               QAbstractSocket::SendBufferSizeSocketOption);

    return true;
}

bool RtpSender::openMulticastInterfaces(int ttl)
{
    const auto interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &iface : interfaces)
    {
        const auto flags = iface.flags();

        if (!flags.testFlag(QNetworkInterface::IsUp) ||
            !flags.testFlag(QNetworkInterface::IsRunning) ||
            flags.testFlag(QNetworkInterface::IsLoopBack) ||
            !flags.testFlag(QNetworkInterface::CanMulticast))
        {
            continue;
        }

        bool hasIPv4 = false;

        for (const QNetworkAddressEntry &entry : iface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                hasIPv4 = true;
                break;
            }
        }

        if (!hasIPv4)
            continue;

        auto *socket = new QUdpSocket(this);

        // Socket BoundState'e geçmeli
        if (!socket->bind(QHostAddress::AnyIPv4,
                          0,
                          QUdpSocket::ShareAddress))
        {
            qWarning() << "Cannot bind multicast socket:"
                       << iface.name()
                       << socket->errorString();

            delete socket;
            continue;
        }

        socket->setSocketOption(
            QAbstractSocket::SendBufferSizeSocketOption,
            16 * 1024 * 1024);

        socket->setSocketOption(
            QAbstractSocket::MulticastTtlOption,
            ttl);

        socket->setSocketOption(
            QAbstractSocket::MulticastLoopbackOption,
            true);

        // Qt6'da dönüş tipi void
        socket->setMulticastInterface(iface);

        m_multicastSockets.append(socket);

        qInfo() << "Multicast interface:"
                << iface.name()
                << iface.humanReadableName();
    }

    if (m_multicastSockets.isEmpty())
    {
        qCritical() << "No suitable multicast interface found";
        return false;
    }

    qInfo() << "Total multicast interfaces:"
            << m_multicastSockets.size();

    return true;
}

void RtpSender::close()
{
    if (m_sendermode == SenderMode::Multicast)
    {


        for (QUdpSocket *socket : std::as_const(m_multicastSockets))
        {
            socket->close();
            delete socket;
        }

        m_multicastSockets.clear();
    }
    else
    {
          m_socket.close();
    }
}

void RtpSender::sendPacket(const RTPPacket &packet)
{

    if(packet.data.isEmpty())
        return;


    if(!m_masterTime)
       {
           qCritical()
               << "RtpSender: MasterTime is NULL!"
               << "this =" << this
               << "thread =" << QThread::currentThread();
           return;
       }

       qint64 now = m_masterTime->nowUs();

    qint64 getpts =packet.pts;

       if (m_sendermode == SenderMode::Multicast)
       {
           //qCritical()<< "RtpSender: Multicast";
           for (QUdpSocket *socket : m_multicastSockets)
           {
               const qint64 written =
                   socket->writeDatagram(packet.data, m_group, m_port);

               if (written != packet.data.size())
               {
                   qWarning() << "Multicast send failed:"
                              << socket->errorString()
                              << "group =" << m_group
                              << "port =" << m_port;
               }
           }
       }
       else
       {

        //qCritical()<< "RtpSender: Unicast";

        for (const auto &d : std::as_const(m_destinations))
        {
            //qCritical()<< "RtpSender: Unicast"<<packet.data.size();
            qint64 written =m_socket.writeDatagram(packet.data,d.address,d.port);
        }
    }
}
void RtpSender::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
   /// qDebug()<<m_masterTime<<"ssssssssssssssssssssssssssssssssss";
}

void RtpSender::addDestination(const QHostAddress &addr,
                                     quint16 port)
{
    for (const auto &d : std::as_const(m_destinations))
    {
        if (d.address == addr &&
            d.port == port)
            return;
    }

    m_destinations.append({addr,port});

    qDebug()
        << "Destination added"
        << addr
        << port;
}

void RtpSender::removeDestination(const QHostAddress &addr,
                                        quint16 port)
{
    for (int i = 0; i < m_destinations.size(); ++i)
    {
        if (m_destinations[i].address == addr &&
            m_destinations[i].port == port)
        {
            m_destinations.removeAt(i);
            break;
        }
    }
}

void RtpSender::clearDestinations()
{
    m_destinations.clear();
}

