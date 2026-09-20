#ifndef VIDEOSENDER_H
#define VIDEOSENDER_H

#include <QObject>
#include <QHostAddress>
#include "sync/mastertime.h"
#include "../common/MediaFrame.h"
#include "../converter/frameconverter.h"
#include "../encoder/h264encoder.h"
#include "../network/rtppacketizer.h"
#include "../sender/rtpsender.h"
#include "../session/sessiondescription.h"
#include "../converter/videoframeconverter.h"

struct VideoClient
{
    QHostAddress address;
    quint16 port;
};

class VideoSender : public QObject
{
    Q_OBJECT

public:

    explicit VideoSender(QObject *parent = nullptr);

    bool open(int width,
              int height,
              int fps,
              int bitrate,
              const QHostAddress &group,
              quint16 port);

    void close();
QList<VideoClient> m_clients;
public slots:

    void onFrame(const MediaFrame &frame);
    void addClient(const QHostAddress &addr, quint16 port);
    void removeClient(const QHostAddress &addr, quint16 port);
    void clearClients();
    void setMasterTime(MasterTime *masterTime);
signals:

    void spsReady(const QByteArray&);
    void ppsReady(const QByteArray&);

private:

    //FrameConverter   m_converter;
    H264Encoder      m_encoder;
    RTPPacketizer    m_packetizer;
    RtpSender  m_sender;

    MediaFrame m_yuvFrame;
    SessionDescription m_session;
    VideoFrameConverter m_converter;
    MasterTime *m_masterTime = nullptr;
};

#endif
