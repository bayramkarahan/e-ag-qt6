#include <QApplication>
#include <QDebug>
#include "stream/senderpipeline.h"
#include "common/StreamSettings.h"
#include "common/commandlineparser.h"
/*
 * *********use********
./mediastreamer --mode sender --source file --file abc.mp4
./mediastreamer --mode sender --source camera --device /dev/video0
./mediastreamer --mode sender --source screen --screen 0
./mediastreamer --mode receiver
*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    StreamSettings settings;
    //settings.mode=StreamMode::Sender;
    //settings.video.source = CaptureSource::Camera;
    settings.video.fileName="***.mp4";
    if (!CommandLineParser::parse(app, settings))
        return -1;

    //qDebug()<<"settings.captureMode"<<int(settings.captureMode);
    //return 0;

    //settings.video.device="/dev/video2";
    ///settings.captureMode = CaptureMode::Video;
    //settings.previewMode = PreviewMode::Remote;
    //settings.videoAddress=QHostAddress("239.10.10.10");
    //settings.videoPort=5004;

    settings.audio.sampleRate=48000;
    settings.audio.channels=2;
    settings.audio.frameSample=960;
    settings.audio.bitrate=20000;
    settings.audioAddress=settings.videoAddress;
    settings.audioPort=settings.videoPort+2;
    /*settings.video.screenIndex = 0;
    settings.video.bitrate=1000;
    settings.video.fps    = 10;
    settings.video.width  = 320;
    settings.video.height = 180;*/
     qCritical() <<"Video Kalitesi:"<<settings.video.bitrate
                <<settings.video.fps
                <<settings.video.width
                <<settings.video.height;

    //settings.video.timeMode = TimeMode::MasterClock;

    SenderPipeline sp;

    if (!sp.open(settings))
    {
        qCritical() << "Failed to initialize MediaStreamer.";
        return -1;
    }

    sp.start();

    return app.exec();
}
