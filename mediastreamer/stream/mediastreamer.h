#ifndef MEDIASTREAMER_H
#define MEDIASTREAMER_H

#include "../common/StreamSettings.h"
#include "../stream/receiverpipeline.h"
#include "../stream/senderpipeline.h"

class MediaStreamer : public QObject
{
    Q_OBJECT

public:
    explicit MediaStreamer(QObject *parent = nullptr);

    bool open(StreamSettings &settings);
    bool start();
    void stop();
    bool prepareSource(StreamSettings &settings);

private:
    SenderPipeline   m_sender;
    ReceiverPipeline m_receiver;

    StreamSettings m_settings;
};

#endif // MEDIASTREAMER_H
