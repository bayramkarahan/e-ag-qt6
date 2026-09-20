#include "videosender.h"

#include <QDebug>

VideoSender::VideoSender(QObject *parent)
    : QObject(parent)
{
    connect(&m_encoder,
            &H264Encoder::packetReady,
            &m_packetizer,
            &RTPPacketizer::packetize,
            Qt::DirectConnection);

    connect(&m_packetizer,
            &RTPPacketizer::packetReady,
            &m_sender,
            &RtpSender::sendPacket,
            Qt::DirectConnection);

   /* connect(&m_encoder,
            &H264Encoder::spsReady,
            &m_session,
            &SessionDescription::setSps);

    connect(&m_encoder,
            &H264Encoder::ppsReady,
            &m_session,
            &SessionDescription::setPps);
*/
    //*******
    connect(&m_encoder,
            &H264Encoder::spsReady,
            this,
            &VideoSender::spsReady);

    connect(&m_encoder,
            &H264Encoder::ppsReady,
            this,
            &VideoSender::ppsReady);
}

bool VideoSender::open(int width,
                       int height,
                       int fps,
                       int bitrate,
                       const QHostAddress &group,
                       quint16 port)
{
    if(!m_encoder.open(width,
                       height,
                       fps,
                       bitrate))
    {
        return false;
    }

    if(!m_sender.open(group,
                      port))
    {
        m_encoder.close();
        return false;
    }
    return true;
}
void VideoSender::setMasterTime(MasterTime *masterTime)
{
    m_masterTime = masterTime;
    m_sender.setMasterTime(masterTime);
}
void VideoSender::close()
{
    m_sender.close();

    m_encoder.close();
}


void VideoSender::onFrame(const MediaFrame &frame)
{

    MediaFrame converted;

    if(!m_converter.convert(frame,converted))
    {
        qDebug()
            << "Frame convert failed";

        return;
    }

   /* qDebug()
        << "Output:"
        << static_cast<int>(converted.format)
        << converted.width
        << converted.height
        << converted.buffer->stride[0]
        << converted.buffer->stride[1]
        << converted.buffer->stride[2];*/

    m_encoder.encode(converted);
}

void VideoSender::addClient(const QHostAddress &addr,
                            quint16 port)
{
      m_sender.addDestination(addr, port);
}

void VideoSender::removeClient(const QHostAddress &addr,
                               quint16 port)
{
      m_sender.removeDestination(addr, port);
}

void VideoSender::clearClients()
{
       m_sender.clearDestinations();
}
