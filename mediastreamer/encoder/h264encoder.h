#ifndef H264ENCODER_H
#define H264ENCODER_H

#include <QObject>

#include "../common/MediaFrame.h"
#include "../common/EncodedPacket.h"
#include <x264.h>


class H264Encoder : public QObject
{
    Q_OBJECT

public:

    explicit H264Encoder(QObject *parent=nullptr);

    ~H264Encoder();

    bool open(int width,
              int height,
              int fps,
              int bitrate = 4000);

    void close();

public slots:

    void encode(const MediaFrame &frame);

signals:

    void packetReady(const EncodedPacket &packet);
    void spsReady(const QByteArray&);
    void ppsReady(const QByteArray&);

private:

    x264_t *m_encoder = nullptr;
    x264_picture_t *m_picture = nullptr;
    int m_width = 0;
    int m_height = 0;
    x264_picture_t m_picIn;
    x264_picture_t m_picOut;
    qint64 m_pts = 0;
};

#endif
