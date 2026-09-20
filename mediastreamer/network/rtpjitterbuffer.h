#ifndef RTPJITTERBUFFER_H
#define RTPJITTERBUFFER_H

#include <QObject>
#include <QMap>

#include "network/rtppacket.h"

class RTPJitterBuffer : public QObject
{
    Q_OBJECT

public:
    explicit RTPJitterBuffer(QObject *parent = nullptr);

    void clear();

public slots:
    void processPacket(const RTPPacket &packet);

signals:
    void packetReady(const RTPPacket &packet);

private:
    void flush();

private:
    QMap<quint16, RTPPacket> m_buffer;

    bool m_initialized = false;
    quint16 m_expectedSequence = 0;

    static constexpr int MAX_BUFFER_PACKETS = 64;

};

#endif // RTPJITTERBUFFER_H
