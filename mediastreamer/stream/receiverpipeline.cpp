#include "receiverpipeline.h"

#include "../receiver/rtpreceiver.h"

#include "../decoder/audioopusdecoder.h"
#include "../decoder/h264depacketizer.h"
#include "../decoder/h264decoder.h"

#include "../player/audioplayback.h"

#include "../preview/videopreview.h"

#include "../sync/mastertime.h"
#include "../sync/mediaclock.h"
#include "../sync/videoscheduler.h"
#include "../sync/audioscheduler.h"

ReceiverPipeline::ReceiverPipeline(QObject *parent)
    : QObject(parent)
{
}

bool ReceiverPipeline::open(const StreamSettings &settings)
{
    m_settings = settings;

    return true;
}

bool ReceiverPipeline::start()
{
    qDebug() << "RUNNING AS RECEIVER";

    m_clock=new MediaClock;

    //m_session=new SessionDescription(this);
    m_audioScheduler=new AudioScheduler(m_clock);
    m_audioScheduler->open(m_settings.video);// zamanlama yapısı gidiyor önemli
    m_videoScheduler=new VideoScheduler(m_clock);
    m_videoScheduler->open(m_settings.video);// zamanlama yapısı gidiyor önemli

    //--------------------------------------------------
    // RENDERER
    //--------------------------------------------------
    m_renderer=new VideoRenderer;
    if(m_settings.previewMode!=PreviewMode::None)
    {
        m_renderer->resize(960,540);
        m_renderer->show();
    }
    m_preview=new VideoPreview(m_renderer);
    m_preview->setMediaClock(m_clock);
    //--------------------------------------------------
    // RECEIVER MODE
    //--------------------------------------------------

    //-------------------------
    // AUDIO RECEIVE
    //-------------------------

    m_audioReceiver=new RTPReceiver;
    if(!m_audioReceiver->open(m_settings.audioAddress,m_settings.audioPort))
        return -1;

    m_audioDecoder=new AudioOpusDecoder;
    if(!m_audioDecoder->open(m_settings.audioSampleRate,m_settings.audioChannels))
        return -1;

    m_playback=new AudioPlayback;

    if(!m_playback->open(m_settings.audioSampleRate,m_settings.audioChannels))
        return -1;

    m_playback->setClock(m_clock);

    m_audioJitter = new RTPJitterBuffer(this);

    connect(m_audioReceiver,&RTPReceiver::packetReceived,
            m_audioJitter,&RTPJitterBuffer::processPacket,
            Qt::QueuedConnection);

    connect(m_audioJitter,&RTPJitterBuffer::packetReady,
            m_audioDecoder,&AudioOpusDecoder::decode,
            Qt::QueuedConnection);




    QObject::connect(m_audioDecoder,&AudioOpusDecoder::frameReady,
                     m_audioScheduler,&AudioScheduler::enqueue,
                     Qt::QueuedConnection);


    QObject::connect(m_audioScheduler,&AudioScheduler::frameReady,
                     m_playback,&AudioPlayback::onFrame,
                     Qt::QueuedConnection);


    //-------------------------
    // VIDEO RECEIVE
    //-------------------------

    m_videoReceiver=new RTPReceiver(this);
    if(!m_videoReceiver->open(m_settings.videoAddress,m_settings.videoPort))
        return -1;

    m_depacketizer=new H264Depacketizer(this);
    m_videoDecoder=new H264Decoder(this);

    if(!m_videoDecoder->open())
        return -1;

  /*  QObject::connect(m_videoReceiver,&RTPReceiver::packetReceived,
                     m_depacketizer,&H264Depacketizer::processPacket,
                     Qt::QueuedConnection);
*/
  /*  QObject::connect(
        m_videoReceiver,
        &RTPReceiver::packetReceived,
        m_depacketizer,
        &H264Depacketizer::processPacket,
        Qt::DirectConnection);*/
    m_videoJitter = new RTPJitterBuffer(this);

    connect(m_videoReceiver,
            &RTPReceiver::packetReceived,
            m_videoJitter,
            &RTPJitterBuffer::processPacket,
            Qt::QueuedConnection);

    connect(m_videoJitter,
            &RTPJitterBuffer::packetReady,
            m_depacketizer,
            &H264Depacketizer::processPacket,
            Qt::QueuedConnection);

  /*  QObject::connect(m_depacketizer,&H264Depacketizer::nalReady,
                     m_videoDecoder,&H264Decoder::decode,
                     Qt::QueuedConnection);*/
    QObject::connect(m_depacketizer,&H264Depacketizer::nalReady,
                     m_videoDecoder,&H264Decoder::decode,
                     Qt::DirectConnection);

    QObject::connect(m_videoDecoder,&H264Decoder::frameReady,
                     m_videoScheduler,&VideoScheduler::enqueue,
                     Qt::QueuedConnection);

    QObject::connect(m_videoScheduler,&VideoScheduler::frameReady,
                     m_preview,&VideoPreview::onFrame,
                     Qt::QueuedConnection);

}

void ReceiverPipeline::stop()
{

}
bool ReceiverPipeline::createObjects()
{

}
