#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class SessionDescription;

class HttpServer : public QObject
{
    Q_OBJECT

public:
    explicit HttpServer(QObject *parent = nullptr);

    bool start(quint16 port);

    void setSession(SessionDescription *session);

private slots:
    void onNewConnection();

private:
    void processRequest(QTcpSocket *socket);

private:
    QTcpServer m_server;
    SessionDescription *m_session = nullptr;
};
