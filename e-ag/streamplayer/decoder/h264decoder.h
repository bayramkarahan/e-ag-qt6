#ifndef H264DECODER_H
#define H264DECODER_H

#include <QObject>

#include "../common/MediaFrame.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

class H264Decoder : public QObject
{
    Q_OBJECT

public:

    explicit H264Decoder(QObject *parent = nullptr);
    ~H264Decoder();

    bool open();
    void close();

public slots:

    void decode(const QByteArray &nal,
                qint64 pts,
                bool marker);
signals:

    void frameReady(const MediaFrame &frame);

private:

    AVCodecContext *m_codecContext = nullptr;
    AVFrame *m_frame = nullptr;
    AVPacket *m_packet = nullptr;

    quint32 m_firstTimestamp = 0;
    bool m_firstTimestampReceived = false;
};

#endif
