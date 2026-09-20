#pragma once

#include <QObject>
#include <QHostAddress>

#include "controlchannel.h"


class ControlServer : public QObject
{
    Q_OBJECT

public:

    explicit ControlServer(
        QObject *parent = nullptr);


    /*
     * Server'ı başlatır.
     *
     * Örneğin:
     *
     * start(6000);
     */
    bool start(
        quint16 localPort = 6000);


    void stop();


    /*
     * Discovery multicast grubuna
     * mesaj gönderir.
     */
    bool broadcast(
        ControlMessageType type,
        const QByteArray &payload =
            QByteArray());


    /*
     * Belirli Client'a unicast mesaj.
     */
    bool sendToClient(
        const QHostAddress &address,
        quint16 port,
        ControlMessageType type,
        const QByteArray &payload =
            QByteArray());


    quint32 serverId() const;


    quint16 localPort() const;


signals:

    /*
     * Discovery mesajları hariç,
     * Client'tan gelen mesajlar.
     */
    void messageReceived(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);


    void errorOccurred(
        const QString &error);


private slots:

    void onMessageReceived(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);


private:

    ControlChannel m_channel;


    /*
     * Discovery multicast adresi.
     */
    QHostAddress m_multicastAddress =
        QHostAddress(
            QStringLiteral(
                "239.255.10.10"));


    /*
     * Discovery multicast portu.
     *
     * Server'ın local portundan bağımsız
     * tutuluyor.
     */
    quint16 m_multicastPort = 6000;


    quint32 m_serverId = 0;

    quint32 m_sequence = 0;
};
