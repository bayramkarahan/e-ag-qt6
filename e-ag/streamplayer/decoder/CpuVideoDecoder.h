#ifndef CPUVIDEODECODER_H
#define CPUVIDEODECODER_H

#include <QObject>

#include "../common/MediaFrame.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}
#include "AbstractVideoDecoder.h"

class CpuVideoDecoder : public AbstractVideoDecoder
{
    Q_OBJECT

public:

    explicit CpuVideoDecoder(QObject *parent = nullptr);
    ~CpuVideoDecoder();


    void setSettings(const StreamSettings &settings);
    bool start() override;
    void stop() override;
    Type type() const override;
public slots:

    void decode(const QByteArray &packet,
                qint64 pts,
                bool marker);


private:

    AVCodecContext *m_codecContext = nullptr;
    AVFrame *m_frame = nullptr;
    AVPacket *m_packet = nullptr;

    quint32 m_firstTimestamp = 0;
    bool m_firstTimestampReceived = false;
        QByteArray m_extraData;
            StreamSettings  m_settings;
};

#endif
