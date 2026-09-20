#include "rtspconnection.h"
#include "../session/sessiondescription.h"

#include <QDebug>

RtspConnection::RtspConnection(QTcpSocket *socket,
                               SessionDescription *session,
                               QObject *parent)
    : QObject(parent),
      m_socket(socket),
      m_session(session)
{
    connect(m_socket,
            &QTcpSocket::readyRead,
            this,
            &RtspConnection::readyRead);

    connect(m_socket,
            &QTcpSocket::disconnected,
            this,
            [this]()
    {
        qDebug() << "RTSP CLIENT DISCONNECTED";
            requestTeardown();
        deleteLater();
    });
    m_clientAddress = socket->peerAddress();
}

void RtspConnection::readyRead()
{
    QByteArray data = m_socket->readAll();

    qDebug().noquote()
            << "\n========== RTSP REQUEST ==========\n"
            << data;

    processRequest(data);
}

void RtspConnection::processRequest(const QByteArray &data)
{
    QString request = QString::fromUtf8(data);

    QString cseq = headerValue(request, "CSeq");
    QString url  = requestUrl(request);

    qDebug() << "METHOD =" << request.section(' ',0,0);
    qDebug() << "URL    =" << url;
    qDebug() << "CSEQ   =" << cseq;

    if(request.startsWith("OPTIONS"))
    {
        sendOptions(cseq);
        return;
    }

    if(request.startsWith("DESCRIBE"))
    {
        sendDescribe(url,cseq);
        return;
    }

    if(request.startsWith("SETUP"))
    {
        QString transport = headerValue(request,"Transport");

        qDebug() << "TRANSPORT =" << transport;

        sendSetup(url,
                  transport,
                  cseq);
        return;
    }

    if(request.startsWith("PLAY"))
    {
        sendPlay(cseq);
        return;
    }

    if(request.startsWith("TEARDOWN"))
    {
        sendTeardown(cseq);
        return;
    }

    QByteArray reply;

    reply += "RTSP/1.0 501 Not Implemented\r\n";
    reply += "CSeq: " + cseq.toUtf8() + "\r\n";
    reply += "Server: MediaStreamer\r\n";
    reply += "\r\n";

    sendResponse(reply);
}

QString RtspConnection::headerValue(const QString &req,
                                    const QString &name) const
{
    QStringList lines = req.split("\r\n");

    for(const QString &line : lines)
    {
        if(line.startsWith(name + ":",Qt::CaseInsensitive))
            return line.section(':',1).trimmed();
    }

    return {};
}

QString RtspConnection::requestUrl(const QString &req) const
{
    QString firstLine = req.section("\r\n",0,0);

    QStringList parts = firstLine.split(' ');

    if(parts.size() >= 2)
        return parts.at(1);

    return {};
}

void RtspConnection::sendResponse(const QByteArray &data)
{
    qDebug().noquote()
        << "\n========== RTSP RESPONSE ==========\n"
        << data;

    m_socket->write(data);
    m_socket->flush();

    if(!m_socket->waitForBytesWritten(3000))
        qDebug() << "write timeout";
}


void RtspConnection::sendOptions(const QString &cseq)
{
    QByteArray reply;

    reply += "RTSP/1.0 200 OK\r\n";
    reply += "CSeq: " + cseq.toUtf8() + "\r\n";
    reply += "Server: MediaStreamer\r\n";
    reply += "Public: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN\r\n";
    reply += "\r\n";

    sendResponse(reply);
}

void RtspConnection::sendDescribe(const QString &url,
                                  const QString &cseq)
{
    QByteArray sdp = m_session->toByteArray();

    if(sdp.isEmpty())
    {
        QByteArray reply;

        reply += "RTSP/1.0 404 Not Found\r\n";
        reply += "CSeq: " + cseq.toUtf8() + "\r\n";
        reply += "Server: MediaStreamer\r\n";
        reply += "\r\n";

        sendResponse(reply);
        return;
    }

    QByteArray reply;

    reply += "RTSP/1.0 200 OK\r\n";
    reply += "CSeq: " + cseq.toUtf8() + "\r\n";

    reply += "Server: MediaStreamer\r\n";

    reply += "Content-Type: application/sdp\r\n";

    reply += "Content-Base: ";
    reply += url.toUtf8();
    reply += "/\r\n";

    reply += "Content-Length: ";
    reply += QByteArray::number(sdp.size());
    reply += "\r\n";

    reply += "Cache-Control: no-cache\r\n";

    reply += "\r\n";

    reply += sdp;

    sendResponse(reply);
}

void RtspConnection::sendSetup(const QString &url,
                               const QString &transport,
                               const QString &cseq)
{
    QString clientPorts =
        transport.section("client_port=", 1, 1)
                 .section(';', 0, 0);

    QByteArray reply;

    reply += "RTSP/1.0 200 OK\r\n";
    reply += "CSeq: " + cseq.toUtf8() + "\r\n";
    reply += "Server: MediaStreamer\r\n";

    reply += "Session: ";
    reply += m_sessionId.toUtf8();
    reply += ";timeout=60\r\n";

    if (url.contains("trackID=0"))
    {
        m_videoClientPorts = clientPorts;

        reply +=
            "Transport: RTP/AVP/UDP;"
            "unicast;"
            "client_port=" +
            clientPorts.toUtf8() +
            ";server_port=5004-5005\r\n";
    }
    else if (url.contains("trackID=1"))
    {
        m_audioClientPorts = clientPorts;

        reply +=
            "Transport: RTP/AVP/UDP;"
            "unicast;"
            "client_port=" +
            clientPorts.toUtf8() +
            ";server_port=5006-5007\r\n";
    }
    else
    {
        qWarning() << "Unknown RTSP track:" << url;

        reply =
            "RTSP/1.0 404 Not Found\r\n"
            "CSeq: " + cseq.toUtf8() + "\r\n"
            "Server: MediaStreamer\r\n"
            "\r\n";

        sendResponse(reply);
        return;
    }

    reply += "\r\n";

    sendResponse(reply);
}

void RtspConnection::sendPlay(const QString &cseq)
{


            QString ip;

            for (const auto &iface : QNetworkInterface::allInterfaces())
            {
                for (const auto &entry : iface.addressEntries())
                {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                        entry.ip() != QHostAddress::LocalHost)
                    {
                        ip = entry.ip().toString();
                        break;
                    }
                }

                if (!ip.isEmpty())
                    break;
            }
    bool hasVideo = !m_videoClientPorts.isEmpty();
    bool hasAudio = !m_audioClientPorts.isEmpty();

    if (!hasVideo && !hasAudio)
    {
        qWarning() << "RTSP: no tracks setup";
        return;
    }

    QByteArray reply;

    reply += "RTSP/1.0 200 OK\r\n";
    reply += "CSeq: " + cseq.toUtf8() + "\r\n";
    reply += "Server: MediaStreamer\r\n";
    reply += "Session: " + m_sessionId.toUtf8() + "\r\n";
    reply += "Range: npt=0.000-\r\n";

    //--------------------------------------------------
    // RTP-Info
    //--------------------------------------------------

    QStringList rtpInfo;

    if (hasVideo)
    {
        rtpInfo << "url=rtsp://"+ip+":8554/stream/trackID=0;"
                   "seq=0;rtptime=0";
    }

    if (hasAudio)
    {
        rtpInfo << "url=rtsp://"+ip+":8554/stream/trackID=1;"
                   "seq=0;rtptime=0";
    }

    reply += "RTP-Info: "
             + rtpInfo.join(',').toUtf8()
             + "\r\n";

    reply += "\r\n";

    sendResponse(reply);

    //--------------------------------------------------
    // CLIENT PORTS
    //--------------------------------------------------

    quint16 videoPort = 0;
    quint16 audioPort = 0;

    if (hasVideo)
    {
        videoPort =
            m_videoClientPorts.section('-', 0, 0).toUShort();
    }

    if (hasAudio)
    {
        audioPort =
            m_audioClientPorts.section('-', 0, 0).toUShort();
    }

    qDebug()
        << "CLIENT =" << m_clientAddress
        << "VIDEO =" << videoPort
        << "AUDIO =" << audioPort;

    emit playRequested(
        m_clientAddress,
        videoPort,
        audioPort);

    qDebug() << "RTSP PLAY";
}

/*

void RtspConnection::sendTeardown(const QString &cseq)
{
    QByteArray reply;

    reply += "RTSP/1.0 200 OK\r\n";
    reply += "CSeq: " + cseq.toUtf8() + "\r\n";
    reply += "Session: " + m_sessionId.toUtf8() + "\r\n";
    reply += "\r\n";

    sendResponse(reply);

    quint16 videoPort = 0;
    quint16 audioPort = 0;

    if (!m_videoClientPorts.isEmpty())
        videoPort = m_videoClientPorts.section('-',0,0).toUShort();

    if (!m_audioClientPorts.isEmpty())
        audioPort = m_audioClientPorts.section('-',0,0).toUShort();

    emit teardownRequested(
        m_clientAddress,
        videoPort,
        audioPort);
}

*/
void RtspConnection::sendTeardown(const QString &cseq)
{
    QByteArray reply;

    reply += "RTSP/1.0 200 OK\r\n";
    reply += "CSeq: " + cseq.toUtf8() + "\r\n";
    reply += "Session: " + m_sessionId.toUtf8() + "\r\n";
    reply += "\r\n";

    sendResponse(reply);

    requestTeardown();
}
void RtspConnection::requestTeardown()
{
    if (m_teardownRequested)
        return;

    m_teardownRequested = true;

    quint16 videoPort = 0;
    quint16 audioPort = 0;

    if (!m_videoClientPorts.isEmpty())
        videoPort = m_videoClientPorts.section('-', 0, 0).toUShort();

    if (!m_audioClientPorts.isEmpty())
        audioPort = m_audioClientPorts.section('-', 0, 0).toUShort();

    if (videoPort == 0 && audioPort == 0)
        return;

    emit teardownRequested(
        m_clientAddress,
        videoPort,
        audioPort);
}
