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

    void processAnnexB(const QByteArray &data,
                       qint64 pts);

signals:
    void nalReady(const QByteArray &nal,
                  qint64 pts,
                  bool marker);

private:
    void processNal(const QByteArray &nal,
                    qint64 pts,
                    bool marker);

    QByteArray m_sps;
    QByteArray m_pps;

    QByteArray m_fuBuffer;


    bool m_fuStarted = false;
    quint16 m_lastSequence = 0;
        bool m_started = false;
};

#endif
