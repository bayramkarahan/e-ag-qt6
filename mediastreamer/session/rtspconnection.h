#ifndef RTSPCONNECTION_H
#define RTSPCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkInterface>

class SessionDescription;

class RtspConnection : public QObject
{
    Q_OBJECT

public:
    explicit RtspConnection(QTcpSocket *socket,
                            SessionDescription *session,
                            QObject *parent=nullptr);

private slots:
    void readyRead();

private:
     void requestTeardown();
    void processRequest(const QByteArray &data);

    QString headerValue(const QString &req,
                        const QString &name) const;

    QString requestUrl(const QString &req) const;

    void sendResponse(const QByteArray &data);

    void sendOptions(const QString &cseq);

    void sendDescribe(const QString &url,
                      const QString &cseq);

    void sendSetup(const QString &url,
                   const QString &transport,
                   const QString &cseq);

    void sendPlay(const QString &cseq);

    void sendTeardown(const QString &cseq);
signals:

    void playRequested(
            QHostAddress address,
            quint16 videoPort,
            quint16 audioPort);

    void teardownRequested(const QHostAddress &address,
                           quint16 videoPort,
                           quint16 audioPort);
private:
    bool m_teardownRequested = false;
    QString m_videoClientPorts;
    QString m_audioClientPorts;

    QHostAddress m_clientAddress;

    QTcpSocket *m_socket=nullptr;

    SessionDescription *m_session=nullptr;

    QString m_sessionId="12345678";

    QString m_videoControl="trackID=0";

    QString m_audioControl="trackID=1";
};

#endif
