#pragma once

#include <QObject>
#include <QHostAddress>
#include <QTimer>

#include "controlchannel.h"


class ControlClient : public QObject
{
    Q_OBJECT

public:

    explicit ControlClient(
        QObject *parent = nullptr);


    /*
     * Client'ı belirtilen local portta başlatır.
     *
     * Örneğin:
     *
     * Client -> 6001
     */
    bool start(
        quint16 localPort = 6001);


    void stop();


    /*
     * Server aramaya başlar.
     *
     * İlk WHO_IS_SERVER hemen gönderilir.
     *
     * Sonrasında 3 saniyede bir
     * tekrar gönderilir.
     *
     * I_AM_SERVER geldiğinde timer durur.
     */
    void discoverServer();


    /*
     * Bulunan Server'a mesaj gönderir.
     */
    bool sendToServer(
        ControlMessageType type,
        const QByteArray &payload =
            QByteArray());


    bool isServerFound() const;


    QHostAddress serverAddress() const;


    quint16 serverPort() const;


    quint32 clientId() const;


    quint16 localPort() const;


signals:

    /*
     * Server bulundu.
     */
    void serverFound(
        const QHostAddress &address,
        quint16 port);


    /*
     * Server'dan mesaj geldi.
     */
    void messageReceived(
        const ControlMessage &message);


    void errorOccurred(
        const QString &error);


private slots:

    void onMessageReceived(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);


    void onDiscoveryTimeout();


private:

    void sendWhoIsServer();


private:

    ControlChannel m_channel;


    /*
     * Discovery multicast.
     */
    QHostAddress m_multicastAddress =
        QHostAddress(
            QStringLiteral(
                "239.255.10.10"));


    /*
     * Discovery'nin ortak UDP portu.
     *
     * Client local portundan farklı olabilir.
     */
    quint16 m_multicastPort = 6000;


    /*
     * Server'ın gerçek adresi.
     */
    QHostAddress m_serverAddress;


    /*
     * Server'ın UDP portu.
     */
    quint16 m_serverPort = 0;


    /*
     * Discovery timer.
     */
    QTimer m_discoveryTimer;


    quint32 m_clientId = 0;

    quint32 m_sequence = 0;


    bool m_serverFound = false;
};
