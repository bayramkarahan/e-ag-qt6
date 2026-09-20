#ifndef H264DEPACKETIZER_H
#define H264DEPACKETIZER_H

#include <QObject>

#include "../network/rtppacket.h"

class H264Depacketizer : public QObject
{
    Q_OBJECT

public:

    explicit H264Depacketizer(QObject *parent = nullptr);


public slots:

    void processPacket(const RTPPacket &packet);

signals:
    void nalReady(const QByteArray &nal,
                  qint64 pts,
                  bool marker);

private:

    QByteArray m_fuBuffer;


    bool m_fuStarted = false;
    quint16 m_lastSequence = 0;
};

#endif
