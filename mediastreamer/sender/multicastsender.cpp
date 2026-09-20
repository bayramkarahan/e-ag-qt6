#include "multicastsender.h"

#include <QDebug>
#include <QAbstractSocket>

MulticastSender::MulticastSender(QObject *parent)
    : QObject(parent)
{
}

bool MulticastSender::open(const QHostAddress &group,
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

void MulticastSender::close()
{
    m_socket.close();
}

void MulticastSender::sendPacket(const RTPPacket &packet)
{

    if(packet.data.isEmpty())
        return;

    ///qDebug().noquote()<< packet.data.left(20).toHex(' ');
    qint64 written =
            m_socket.writeDatagram(packet.data,
                                   m_group,
                                   m_port);


    if(written != packet.data.size())
    {
       // qWarning() << "writeDatagram failed:"
                ///   << m_socket.errorString();
    }
}
