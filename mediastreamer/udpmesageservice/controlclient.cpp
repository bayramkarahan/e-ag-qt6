#include "controlclient.h"

#include <QRandomGenerator>
#include <QDebug>


ControlClient::ControlClient(
    QObject *parent)
    : QObject(parent)
{
    /*
     * Client ID.
     */
    m_clientId =
        QRandomGenerator::global()
            ->generate();


    /*
     * Discovery:
     *
     * 3 saniyede bir.
     */
    m_discoveryTimer.setInterval(
        3000);


    connect(
        &m_discoveryTimer,
        &QTimer::timeout,
        this,
        &ControlClient::onDiscoveryTimeout);


    connect(
        &m_channel,
        &ControlChannel::messageReceived,
        this,
        &ControlClient::onMessageReceived);


    connect(
        &m_channel,
        &ControlChannel::errorOccurred,
        this,
        &ControlClient::errorOccurred);
}


bool ControlClient::start(
    quint16 localPort)
{
    /*
     * Örneğin:
     *
     * Client localPort = 6001
     */
    if (!m_channel.bind(localPort))
        return false;


    /*
     * Discovery multicast grubuna katıl.
     */
    if (!m_channel.joinMulticast(
            m_multicastAddress))
    {
        m_channel.close();

        return false;
    }


    qInfo()
        << "ControlClient started";


    qInfo()
        << "Client ID:"
        << m_clientId;


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


void ControlClient::stop()
{
    /*
     * Discovery timer'ı kesinlikle durdur.
     */
    m_discoveryTimer.stop();


    m_channel.leaveMulticast(
        m_multicastAddress);


    m_channel.close();


    m_serverFound = false;

    m_serverAddress.clear();

    m_serverPort = 0;
}


void ControlClient::discoverServer()
{
    /*
     * Zaten Server bulunduysa
     * tekrar discovery başlatma.
     */
    if (m_serverFound)
    {
        qInfo()
            << "Server already found:"
            << m_serverAddress.toString()
            << ":"
            << m_serverPort;

        return;
    }


    /*
     * İlk mesajı hemen gönder.
     */
    sendWhoIsServer();


    /*
     * Bundan sonra:
     *
     * 3 saniye
     * 3 saniye
     * 3 saniye
     *
     * ...
     */
    m_discoveryTimer.start();


    qInfo()
        << "Server discovery started";
}


void ControlClient::sendWhoIsServer()
{
    if (m_serverFound)
        return;


    ControlMessage message;

    message.type =
        ControlMessageType::WhoIsServer;

    message.sequence =
        m_sequence++;

    message.senderId =
        m_clientId;


    /*
     * Burada destination:
     *
     * 239.255.10.10:6000
     *
     * Client'ın local portu 6001 olsa
     * bile fark etmez.
     */
    const bool ok =
        m_channel.sendMulticast(
            m_multicastAddress,
            m_multicastPort,
            message);


    if (ok)
    {
        qInfo()
            << "WHO_IS_SERVER sent";
    }
}


void ControlClient::onDiscoveryTimeout()
{
    /*
     * Her ihtimale karşı.
     */
    if (m_serverFound)
    {
        m_discoveryTimer.stop();

        return;
    }


    /*
     * 3 saniye doldu.
     *
     * Tekrar Server sor.
     */
    sendWhoIsServer();
}


void ControlClient::onMessageReceived(
    const QHostAddress &address,
    quint16 port,
    const ControlMessage &message)
{
    /*
     * Server kendisini tanıttı.
     */
    if (message.type ==
        ControlMessageType::IAmServer)
    {
        /*
         * Daha önce Server bulduysak
         * ikinci kez işlemiyoruz.
         */
        if (m_serverFound)
            return;


        /*
         * Server bulundu.
         */
        m_serverFound = true;


        /*
         * Server'ın IP'si.
         */
        m_serverAddress =
            address;


        /*
         * Server'ın gönderdiği source port.
         *
         * Normalde 6000.
         */
        m_serverPort =
            port;


        /*
         * ARTIK DISCOVERY BİTTİ.
         *
         * 3 saniyelik timer duruyor.
         */
        m_discoveryTimer.stop();


        qInfo()
            << "================================";


        qInfo()
            << "SERVER FOUND";


        qInfo()
            << "Server address:"
            << m_serverAddress.toString();


        qInfo()
            << "Server port:"
            << m_serverPort;


        qInfo()
            << "Server ID:"
            << message.senderId;


        qInfo()
            << "Discovery stopped";


        qInfo()
            << "================================";


        emit serverFound(
            m_serverAddress,
            m_serverPort);


        return;
    }


    /*
     * I_AM_SERVER dışındaki bütün
     * mesajları uygulamaya gönder.
     */
    emit messageReceived(message);
}


bool ControlClient::sendToServer(
    ControlMessageType type,
    const QByteArray &payload)
{
    if (!m_serverFound)
    {
        qWarning()
            << "Cannot send message:"
            << "server not found";

        return false;
    }


    ControlMessage message;

    message.type = type;

    message.sequence =
        m_sequence++;

    message.senderId =
        m_clientId;

    message.payload =
        payload;


    /*
     * Bundan sonra artık multicast yok.
     *
     * Doğrudan:
     *
     * Client -> Server
     *
     * unicast.
     */
    return m_channel.sendUnicast(
        m_serverAddress,
        m_serverPort,
        message);
}


bool ControlClient::isServerFound() const
{
    return m_serverFound;
}


QHostAddress ControlClient::serverAddress() const
{
    return m_serverAddress;
}


quint16 ControlClient::serverPort() const
{
    return m_serverPort;
}


quint32 ControlClient::clientId() const
{
    return m_clientId;
}


quint16 ControlClient::localPort() const
{
    return m_channel.localPort();
}
