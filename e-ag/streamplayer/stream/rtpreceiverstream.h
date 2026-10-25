#ifndef RTPRECEIVERSTREAM_H
#define RTPRECEIVERSTREAM_H
#include <QObject>
#include <QHostAddress>
#include<QScreen>
#include "../common/StreamSettings.h"
#include "../receiver/rtpsource.h"
#include "../receiver/rtpworker.h"
class RTPReceiver;
class H264Depacketizer;
class H264Decoder;
class AudioOpusDecoder;
class RtpAudioPlayback;
class VideoRenderer;
class VideoPreview;
class OpenGLVideoRenderer;
class OpenGLVaapiZeroCopyRenderer;
class OpenGLVideoPreview;
class RtpMasterTime;
class RtpMediaClock;
class RtpVideoScheduler;
class RtpAudioScheduler;
class AbstractVideoDecoder;
class CpuVideoDecoder;
class RTPJitterBuffer;
class RtpReceiverStream: public QObject
{
    Q_OBJECT
public:

 explicit RtpReceiverStream(QObject *parent = nullptr);
    bool open(const StreamSettings &settings);
    bool start1();
    bool start();
    void tryStartPlayback();
    bool startAudio();
    bool startVideo();
    bool startAudioVideo();
VideoPreview*      m_preview = nullptr;
private:
         StreamSettings m_settings;

        RtpMasterTime*        m_masterTime = nullptr;
        RtpMediaClock*        m_clock = nullptr;


            //OpenGLVaapiZeroCopyRenderer*     m_openglvaapizerorenderer = nullptr;

            OpenGLVideoPreview*      m_openglpreview = nullptr;

            RTPReceiver*       m_videoReceiver = nullptr;
            RTPReceiver*       m_audioReceiver = nullptr;

            H264Depacketizer*  m_h264Depacketizer = nullptr;
            //H264Decoder*       m_videoDecoder = nullptr;
            AbstractVideoDecoder *m_videoDecoder = nullptr;
            //CpuVideoDecoder *m_videoDecoder = nullptr;

            AudioOpusDecoder*  m_audioDecoder = nullptr;

            RtpVideoScheduler*    m_videoScheduler = nullptr;
            RtpAudioScheduler*    m_audioScheduler = nullptr;

            RtpAudioPlayback*     m_playback = nullptr;
            RTPJitterBuffer *m_videoJitterBuffer=nullptr;
            RTPJitterBuffer *m_audioJitterBuffer=nullptr;
            RtpSource   *m_rtpSource=nullptr;
            bool m_decoderStarted = false;

            bool m_playbackSynchronized = false;
            bool m_videoStarted = false;
            bool m_audioStarted = false;
            bool m_playbackStarted = false;

            qint64 m_firstVideoPts = -1;
            qint64 m_firstAudioPts = -1;

};

#endif // RTPRECEIVERSTREAM_H
