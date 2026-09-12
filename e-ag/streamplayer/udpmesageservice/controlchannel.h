#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#include "controlmessage.h"


class ControlChannel : public QObject
{
    Q_OBJECT

public:

    explicit ControlChannel(
        QObject *parent = nullptr);


    /*
     * UDP socket'i belirtilen local portta açar.
     *
     * Örnek:
     *
     * Server -> 6000
     * Client -> 6001
     */
    bool bind(quint16 localPort);


    void close();


    /*
     * Multicast grubuna katılır.
     */
    bool joinMulticast(
        const QHostAddress &address);


    /*
     * Multicast grubundan ayrılır.
     */
    bool leaveMulticast(
        const QHostAddress &address);


    /*
     * Multicast mesajı gönderir.
     *
     * Hedef:
     *
     * multicastAddress
     * multicastPort
     */
    bool sendMulticast(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);


    /*
     * Belirli IP + porta UDP mesajı gönderir.
     */
    bool sendUnicast(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);


    bool isBound() const;


    quint16 localPort() const;


signals:

    /*
     * UDP paketi geldi.
     *
     * address = gönderen IP
     * port    = gönderen UDP portu
     */
    void messageReceived(
        const QHostAddress &address,
        quint16 port,
        const ControlMessage &message);


    void errorOccurred(
        const QString &error);


private slots:

    void onReadyRead();


private:

    QUdpSocket m_socket;

    quint16 m_localPort = 0;
};
