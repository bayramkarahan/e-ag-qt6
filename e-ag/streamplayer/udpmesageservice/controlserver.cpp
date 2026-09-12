#include "controlserver.h"

#include <QRandomGenerator>
#include <QDebug>


ControlServer::ControlServer(
    QObject *parent)
    : QObject(parent)
{
    /*
     * Server için rastgele ID.
     */
    m_serverId =
        QRandomGenerator::global()
            ->generate();


    connect(
        &m_channel,
        &ControlChannel::messageReceived,
        this,
        &ControlServer::onMessageReceived);


    connect(
        &m_channel,
        &ControlChannel::errorOccurred,
        this,
        &ControlServer::errorOccurred);
}


bool ControlServer::start(
    quint16 localPort)
{
    /*
     * Örneğin:
     *
     * Server localPort = 6000
     */
    if (!m_channel.bind(localPort))
        return false;


    /*
     * Server discovery multicast grubuna
     * katılır.
     */
    if (!m_channel.joinMulticast(
            m_multicastAddress))
    {
        m_channel.close();

        return false;
    }


    qInfo()
        << "ControlServer started";


    qInfo()
        << "Server ID:"
        << m_serverId;


    qInfo()
        << "Local port:"
        << m_channel.localPort();


    qInfo()
        << "Discovery multicast:"
        << m_multicastAddress.toString()
        << ":"
        << m_multicastPort;


    return true;
}


void ControlServer::stop()
{
    m_channel.leaveMulticast(
        m_multicastAddress);


    m_channel.close();
}


bool ControlServer::broadcast(
    ControlMessageType type,
    const QByteArray &payload)
{
    ControlMessage message;

    message.type = type;

    message.sequence =
        m_sequence++;

    message.senderId =
        m_serverId;

    message.payload =
        payload;


    return m_channel.sendMulticast(
        m_multicastAddress,
        m_multicastPort,
        message);
}


bool ControlServer::sendToClient(
    const QHostAddress &address,
    quint16 port,
    ControlMessageType type,
    const QByteArray &payload)
{
    ControlMessage message;

    message.type = type;

    message.sequence =
        m_sequence++;

    message.senderId =
        m_serverId;

    message.payload =
        payload;


    return m_channel.sendUnicast(
        address,
        port,
        message);
}


quint32 ControlServer::serverId() const
{
    return m_serverId;
}


quint16 ControlServer::localPort() const
{
    return m_channel.localPort();
}


void ControlServer::onMessageReceived(
    const QHostAddress &address,
    quint16 port,
    const ControlMessage &message)
{
    /*
     * Client Server arıyor.
     */
    if (message.type ==
        ControlMessageType::WhoIsServer)
    {
        qInfo()
            << "================================";


        qInfo()
            << "WHO_IS_SERVER received";


        qInfo()
            << "Client:"
            << address.toString()
            << ":"
            << port;


        /*
         * Client'ın gönderdiği source portu
         * kullanıyoruz.
         *
         * Örneğin:
         *
         * Client = 6001
         *
         * O halde:
         *
         * I_AM_SERVER -> 6001
         */
        sendToClient(
            address,
            port,
            ControlMessageType::IAmServer);


        qInfo()
            << "I_AM_SERVER sent to"
            << address.toString()
            << ":"
            << port;


        qInfo()
            << "================================";


        return;
    }


    /*
     * Discovery dışındaki mesajları
     * uygulamaya gönder.
     */
    emit messageReceived(
        address,
        port,
        message);
}
