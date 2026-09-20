#include "httpserver.h"
#include "sessiondescription.h"

#include <QDebug>

HttpServer::HttpServer(QObject *parent)
    : QObject(parent)
{
    connect(&m_server,
            &QTcpServer::newConnection,
            this,
            &HttpServer::onNewConnection);
}

bool HttpServer::start(quint16 port)
{
    if(!m_server.listen(QHostAddress::Any, port))
    {
        qDebug() << "HTTP listen failed";
        return false;
    }

    qDebug() << "HTTP Server listening on port" << port;

    return true;
}

void HttpServer::setSession(SessionDescription *session)
{
    m_session = session;
}

void HttpServer::onNewConnection()
{
    while(m_server.hasPendingConnections())
    {
        QTcpSocket *socket = m_server.nextPendingConnection();

        connect(socket,
                &QTcpSocket::readyRead,
                this,
                [this, socket]()
                {
                    processRequest(socket);
                });

        connect(socket,
                &QTcpSocket::disconnected,
                socket,
                &QObject::deleteLater);
    }
}

void HttpServer::processRequest(QTcpSocket *socket)
{
    QByteArray request = socket->readAll();

    qDebug().noquote() << request;
    ///if(!request.startsWith("GET /stream.sdp"))
   /* /stream        → Video + Audio SDP
    /video         → Sadece video SDP
    /audio         → Sadece audio SDP
    /status        → JSON durum bilgisi
    /info          → Yayın bilgileri
    bunlar eklenebilir
    */

    if(!request.startsWith("GET /stream"))
    {
        QByteArray reply;

        reply += "HTTP/1.1 404 Not Found\r\n";
        reply += "Connection: close\r\n";
        reply += "\r\n";

        socket->write(reply);
        socket->disconnectFromHost();

        return;
    }

    if(!m_session)
    {
        QByteArray reply;

        reply += "HTTP/1.1 500 Internal Server Error\r\n";
        reply += "Connection: close\r\n";
        reply += "\r\n";

        socket->write(reply);
        socket->disconnectFromHost();

        return;
    }

    QByteArray body = m_session->toByteArray();

    QByteArray header;

    header += "HTTP/1.1 200 OK\r\n";
    header += "Content-Type: application/sdp\r\n";
    header += "Content-Length: ";
    header += QByteArray::number(body.size());
    header += "\r\n";
    header += "Connection: close\r\n";
    header += "\r\n";

    qDebug() << "SDP requested from"
             << socket->peerAddress().toString();

    socket->write(header);
    socket->write(body);

    socket->disconnectFromHost();
}
