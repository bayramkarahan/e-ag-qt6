#include "mediastreamer.h"

MediaStreamer::MediaStreamer(QObject *parent)
    : QObject(parent)
{
}

bool MediaStreamer::open(StreamSettings &settings)
{
    //
    if(settings.mode==StreamMode::Sender)
    {
        settings.video.timeMode = TimeMode::MasterClock;
    if (!prepareSource(settings))
        return false;

        /*qDebug()<<"xxxxxxxxxxxxxxxxxxxxxx";
        qDebug()<<"camera settings "<<
                  m_settings.video.width<<
                   m_settings.video.height<<
                  m_settings.video.fps;*/
        return m_sender.open(m_settings);
    }

    if (settings.mode == StreamMode::Receiver)
    {  m_settings = settings;
        if (settings.video.source == CaptureSource::MediaFile){
            m_settings.video.timeMode = TimeMode::FramePts;
          }

        qDebug()<<"m_settings.video.timeMode"<<int(m_settings.video.timeMode);
        qDebug()<<"CaptureSource::MediaFile"<<int(CaptureSource::MediaFile);
        qDebug()<<"settings.video.source"<<int(settings.video.source);

        return m_receiver.open(m_settings);
    }
    return true;
}

bool MediaStreamer::start()
{
    if (m_settings.mode == StreamMode::Sender)
        return m_sender.start();

    return m_receiver.start();
}

bool MediaStreamer::prepareSource(StreamSettings &settings)
{

    if (settings.video.source == CaptureSource::MediaFile){

        MediaFileInfo info =
                MediaFileInfoReader::read(settings.video.fileName);

        if (!info.valid)
        {
            qDebug() << "Dosya açılamadı.";
            return false;
        }

       qDebug() << "Video :" << info.hasVideo;
        qDebug() << "Audio :" << info.hasAudio;
        qDebug() << "Size   :" << info.width << "x" << info.height;
        qDebug() << "FPS    :" << info.fps;
        qDebug() << "VCodec :" << info.videoCodec;
        qDebug() << "ACodec :" << info.audioCodec;
        qDebug() << "Rate   :" << info.sampleRate;
        qDebug() << "Channel:" << info.channels;

        settings.video.width  = info.width;
        settings.video.height = info.height;
        settings.video.timeMode = TimeMode::FramePts;
        settings.video.fps    = info.fps;
        //settings.audioSampleRate=info.sampleRate;
        //settings.audioFrameSample=882;
        settings.audioChannels=info.channels;
    }
    else if (settings.video.source == CaptureSource::Camera){

        CameraInfo info =
            CameraInfoReader::read(settings.video.device);

        if (!info.valid)
        {
            qDebug() << "Camera not found.";
            return false;
        }

       /* qDebug() << "Name   :" << info.name;
        qDebug() << "Driver :" << info.driver;
        qDebug() << "Bus    :" << info.bus;
        qDebug() << "Size   :" << info.width << "x" << info.height;
        qDebug() << "FPS    :" << info.fps;
        qDebug() << "Format :" << info.pixelFormat;*/

        ///settings.video.width  = info.width;
        ///settings.video.height = info.height;
        //settings.video.fps    = info.fps;

        if(settings.captureMode == CaptureMode::Video)
            settings.video.timeMode = TimeMode::FramePts;
    }
    else{
        const auto screens = QGuiApplication::screens();

        if (settings.video.screenIndex < 0 ||
            settings.video.screenIndex >= screens.size())
        {
            qCritical() << "Invalid screen index";
            return false;
        }

        auto screen = screens.at(settings.video.screenIndex);

        //auto screen = QGuiApplication::screens().at(settings.video.screenIndex);

        int width  = screen->geometry().width();
        int height = screen->geometry().height();
        //qDebug() << "width:" << width;
        //qDebug() << "height:" << height;

        settings.video.width  = width;
        settings.video.height = height;
        if(settings.captureMode == CaptureMode::Video)
            settings.video.timeMode = TimeMode::FramePts;
    }
    m_settings = settings;
    return true;
}


void MediaStreamer::stop()
{
    if (m_settings.mode == StreamMode::Sender)
        m_sender.stop();
    else
        m_receiver.stop();
}
