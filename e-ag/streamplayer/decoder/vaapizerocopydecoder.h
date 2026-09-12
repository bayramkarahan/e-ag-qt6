#ifndef VAAPIZEROVIDEODECODER_H
#define VAAPIZEROVIDEODECODER_H
#include "../common/hardwareframe.h"
#include "../common/vaapihardwareframe.h"
#include "../common/MediaFrame.h"

#include "AbstractVideoDecoder.h"
#include<QQueue>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/hwcontext.h>
}
extern "C"
{
#include <libavutil/hwcontext.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}
extern "C" {
#include <va/va.h>
#include <va/va_drmcommon.h>
#include <libavutil/hwcontext_vaapi.h>
}

class VaapiZeroCopyDecoder : public AbstractVideoDecoder
{
    Q_OBJECT

public:

    explicit VaapiZeroCopyDecoder(QObject *parent = nullptr);

    ~VaapiZeroCopyDecoder();

    static bool isSupported();
    void setSettings(const StreamSettings &settings);
    bool start() override;
    void stop() override;
    Type type() const override;
    void decodePacket(const QByteArray &nal,
                                        qint64 pts);
public slots:

    void decode(const QByteArray &nal,
                qint64 pts,
                bool marker) override;

private:

    static enum AVPixelFormat getHwFormat(
        AVCodecContext *ctx,
        const enum AVPixelFormat *pix_fmts);

    AVCodecContext *m_codecContext = nullptr;

    AVFrame *m_frame = nullptr;

    AVFrame *m_hwFrame = nullptr;

    AVPacket *m_packet = nullptr;

    AVBufferRef *m_hwDevice = nullptr;

    AVFrame *m_swFrame = nullptr;

    bool m_decoderReady = false;
    bool m_started = false;


    QByteArray m_sps;
    QByteArray m_pps;

    SwsContext *m_sws = nullptr;
    StreamSettings  m_settings;

};

#endif
