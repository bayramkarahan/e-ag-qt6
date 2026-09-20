#include "rtspserver.h"
#include "rtspconnection.h"
#include "../session/sessiondescription.h"

RtspServer::RtspServer(QObject *parent)
    : QObject(parent)
{
    connect(&m_server,
            &QTcpServer::newConnection,
            this,
            &RtspServer::newConnection);

}

void RtspServer::setSession(SessionDescription *session)
{
    m_session=session;
}

bool RtspServer::start(quint16 port)
{
    return m_server.listen(QHostAddress::Any,port);
}

void RtspServer::newConnection()
{
    while (m_server.hasPendingConnections())
    {
        QTcpSocket *socket = m_server.nextPendingConnection();

        /*new RtspConnection(socket,
                           m_session,
                           this);*/
        RtspConnection *c =
                new RtspConnection(socket,m_session,this);

        connect(c,
                &RtspConnection::playRequested,
                this,
                &RtspServer::playRequested);
        connect(c,
                &RtspConnection::teardownRequested,
                this,
                &RtspServer::teardownRequested);
    }
}
