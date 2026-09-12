#include "controlchannel.h"

#include <QNetworkDatagram>
#include <QDebug>


ControlChannel::ControlChannel(
    QObject *parent)
    : QObject(parent)
{
    connect(
        &m_socket,
        &QUdpSocket::readyRead,
        this,
        &ControlChannel::onReadyRead);


    connect(
        &m_socket,
        &QUdpSocket::errorOccurred,
        this,
        [this](QAbstractSocket::SocketError)
        {
            emit errorOccurred(
                m_socket.errorString());
        });
}


bool ControlChannel::bind(
    quint16 localPort)
{
    if (isBound())
    {
        qWarning()
            << "ControlChannel already bound";

        return false;
    }


    /*
     * Aynı bilgisayarda örneğin:
     *
     * Server -> 6000
     * Client -> 6001
     *
     * kullanılabilir.
     *
     * Ayrıca multicast socket'leri için
     * ShareAddress + ReuseAddressHint
     * kullanıyoruz.
     */
    const bool ok =
        m_socket.bind(
            QHostAddress::AnyIPv4,
            localPort,
            QUdpSocket::ShareAddress |
            QUdpSocket::ReuseAddressHint);


    if (!ok)
    {
        qWarning()
            << "ControlChannel bind failed:"
            << m_socket.errorString();

        emit errorOccurred(
            m_socket.errorString());

        return false;
    }


    m_localPort =
        m_socket.localPort();


    qInfo()
        << "ControlChannel bound to port"
        << m_localPort;


    return true;
}


void ControlChannel::close()
{
    m_socket.close();

    m_localPort = 0;
}


bool ControlChannel::joinMulticast(
    const QHostAddress &address)
{
    if (!isBound())
        return false;


    const bool ok =
        m_socket.joinMulticastGroup(
            address);


    if (!ok)
    {
        qWarning()
            << "Failed to join multicast:"
            << address.toString();

        qWarning()
            << m_socket.errorString();

        emit errorOccurred(
            m_socket.errorString());
    }


    return ok;
}


bool ControlChannel::leaveMulticast(
    const QHostAddress &address)
{
    if (!isBound())
        return false;


    return m_socket.leaveMulticastGroup(
        address);
}


bool ControlChannel::sendMulticast(
    const QHostAddress &address,
    quint16 port,
    const ControlMessage &message)
{
    if (!isBound())
        return false;


    const QByteArray data =
        serializeControlMessage(message);


    /*
     * ÖNEMLİ:
     *
     * Burada artık m_localPort kullanılmıyor.
     *
     * Multicast destination port dışarıdan
     * veriliyor.
     *
     * Örneğin:
     *
     * 239.255.10.10:6000
     */
    const qint64 result =
        m_socket.writeDatagram(
            data,
            address,
            port);


    if (result < 0)
    {
        emit errorOccurred(
            m_socket.errorString());

        return false;
    }


    return result == data.size();
}


bool ControlChannel::sendUnicast(
    const QHostAddress &address,
    quint16 port,
    const ControlMessage &message)
{
    if (!isBound())
        return false;


    const QByteArray data =
        serializeControlMessage(message);


    /*
     * Burada hedef tamamen gelen
     * address + port değeridir.
     */
    const qint64 result =
        m_socket.writeDatagram(
            data,
            address,
            port);


    if (result < 0)
    {
        emit errorOccurred(
            m_socket.errorString());

        return false;
    }


    return result == data.size();
}


bool ControlChannel::isBound() const
{
    return m_socket.state() ==
           QAbstractSocket::BoundState;
}


quint16 ControlChannel::localPort() const
{
    return m_localPort;
}


void ControlChannel::onReadyRead()
{
    while (m_socket.hasPendingDatagrams())
    {
        const QNetworkDatagram datagram =
            m_socket.receiveDatagram();


        if (!datagram.isValid())
            continue;


        ControlMessage message;


        if (!deserializeControlMessage(
                datagram.data(),
                message))
        {
            qWarning()
                << "Invalid control message from"
                << datagram.senderAddress()
                       .toString()
                << ":"
                << datagram.senderPort();

            continue;
        }


        /*
         * Buradaki address + port çok önemli.
         *
         * Örneğin Client 6001'den gönderiyorsa:
         *
         * address = 127.0.0.1
         * port    = 6001
         *
         * Server bu bilgiyi kullanarak
         * I_AM_SERVER mesajını doğrudan
         * Client'a gönderebilir.
         */
        emit messageReceived(
            datagram.senderAddress(),
            datagram.senderPort(),
            message);
    }
}
