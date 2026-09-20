#include "rtpsender.h"

#include <QDebug>
#include <QAbstractSocket>

RtpSender::RtpSender(QObject *parent)
    : QObject(parent)
{
}

bool RtpSender::open(const QHostAddress &group,
                           quint16 port,
                           int ttl)
{
    close();

    m_group = group;
    m_port = port;
/*
    if(!m_socket.bind(QHostAddress::AnyIPv4, 0,
                      QUdpSocket::ShareAddress))
    {
        qCritical() << "Cannot bind sender:"
                    << m_socket.errorString();

        return false;
    }
*/
    m_socket.setSocketOption(
        QAbstractSocket::SendBufferSizeSocketOption,
        16 * 1024 * 1024);

    qDebug()
        << "Send buffer ="
        << m_socket.socketOption(
               QAbstractSocket::SendBufferSizeSocketOption);

    m_socket.setSocketOption(QAbstractSocket::MulticastTtlOption, ttl);
    m_socket.setSocketOption(QAbstractSocket::MulticastLoopbackOption, true);

    /*m_socket.setSocketOption(
                QAbstractSocket::MulticastLoopbackOption,
                QVariant(true));

    m_socket.setSocketOption(
                QAbstractSocket::MulticastLoopbackOption,
                1);*/

    qDebug() << "Multicast sender:"
             << group.toString()
             << port;

    return true;
}

void RtpSender::close()
{
    m_socket.close();
}

void RtpSender::sendPacket(const RTPPacket &packet)
{

    if(packet.data.isEmpty())
        return;
   /*  qDebug()
        << "RTP SENDER IN"
        << "seq =" << packet.sequence
        << "ts =" << packet.timestamp
        << "pts =" << packet.pts
        << "size =" << packet.data.size();
    ///qDebug().noquote()<< packet.data.left(20).toHex(' ');
   qint64 written =
            m_socket.writeDatagram(packet.data,
                                   m_group,
                                   m_port);*/

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
   /* */

    for (const auto &d : std::as_const(m_destinations))
    {
        qint64 written =m_socket.writeDatagram(packet.data,
                               d.address,
                               d.port);
        /*qDebug()
                << "UDP SEND"
                << "pts =" << getpts
                << "delay =" << (now - getpts)
                << "thread =" << QThread::currentThread();*/
       /* qDebug()
            << "UDP SEND"
            << "written =" << written
            << "seq =" << packet.sequence
            << "ts =" << packet.timestamp
            << "target =" << d.address
            << d.port;*/
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

