#pragma once

#include <QObject>
#include <QTcpServer>

class SessionDescription;

class RtspServer : public QObject
{
    Q_OBJECT

public:
    explicit RtspServer(QObject *parent = nullptr);

    bool start(quint16 port = 8554);
    void setSession(SessionDescription *session);
private slots:
    void newConnection();
signals:

    void playRequested(
            QHostAddress,
            quint16,
            quint16);
    void teardownRequested(const QHostAddress &address,
                           quint16 videoPort,
                           quint16 audioPort);
private:
    QTcpServer m_server;
    SessionDescription *m_session = nullptr;
};
