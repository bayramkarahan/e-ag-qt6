#include "commandlineparser.h"

#include <QCommandLineParser>
#include <QHostAddress>
#include <QDebug>


VideoQualityProfile CommandLineParser::applyVideoQuality(int quality)
{
    static constexpr VideoQualityProfile profiles[] = {
        {  100,  160, 120,  5 }, //  0
        {  150,  320, 240,  8 }, // 10
        {  250,  320, 240, 10 }, // 20
        {  350,  640, 480,  8 }, // 30
        {  500,  640, 480, 10 }, // 40
        {  650,  800, 600, 10 }, // 50
        {  800,  800, 600, 12 }, // 60
        { 1000, 1024, 768, 12 }, // 70
        { 1200, 1280, 768, 12 }, // 80
        { 1600, 1280, 960, 15 }, // 90
        { 2500, 1920,1080, 20 }  // 100
    };

    quality = qBound(0, quality, 100);

    const int index = qBound(0, (quality + 5) / 10, 10);
    const auto &p = profiles[index];

    return p;

}


bool CommandLineParser::parse(QApplication &app,
                              StreamSettings &settings)
{
    QCommandLineParser parser;

    parser.setApplicationDescription("MediaStreamer");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption({
        "mode",
        "sender | receiver",
        "mode"
    });

    parser.addOption({
        "sendermode",
        "unicast | multicast",
        "sendermode"
    });

    parser.addOption({
        "videosource",
        "file | camera | screen | none",
        "videosource"
    });

    parser.addOption({
        "audiosource",
        "file | microphone | system | both | none",
        "audiosource"
    });
    parser.addOption({
        "quality",
        "0-100",
        "quality"
    });
    parser.addOption({
        "file",
        "Video file",
        "file"
    });

    parser.addOption({
        "device",
        "Camera device",
        "device"
    });

    parser.addOption({
        "screen",
        "Screen index",
        "screen"
    });



    parser.addOption({
        "address",
        "Multicast address",
        "address"
    });

    parser.addOption({
        "video-port",
        "Video RTP port",
        "port"
    });

    parser.process(app);

    //----------------------------------------------------
    // Mode
    //----------------------------------------------------

    QString mode = parser.value("mode").toLower();

    if (mode == "sender")
        settings.mode = StreamMode::Sender;
    else if (mode == "receiver")
        settings.mode = StreamMode::Receiver;
    else
    {
        qCritical() << "Invalid mode.";
        parser.showHelp(1);
    }

    //----------------------------------------------------
    // SenderMode
    //----------------------------------------------------

    QString sendermode = parser.value("sendermode").toLower();

    if (sendermode == "unicast")
        settings.sendermode = SenderMode::Unicast;
    else if (sendermode == "multicast")
        settings.sendermode = SenderMode::Multicast;
    else
        settings.sendermode = SenderMode::Unicast;

    QString quality = parser.value("quality").toLower();
    VideoQualityProfile vqp;
    if (quality != "")
        vqp=applyVideoQuality(quality.toUInt());
    else
        vqp=applyVideoQuality(settings.quality);

    settings.video.bitrate=vqp.bitrate;
    settings.video.fps=vqp.fps;
    settings.video.width=vqp.width;
    settings.video.height=vqp.height;


    //----------------------------------------------------
    // audiosource
    //----------------------------------------------------

    QString audiosource = parser.value("audiosource").toLower();

    if (audiosource == "file")
        settings.audio.source = AudioSource::MediaFile;
    else if (audiosource == "microphone")
        settings.audio.source = AudioSource::Microphone;
    else if (audiosource == "system")
        settings.audio.source = AudioSource::System;
    else if (audiosource == "both")
        settings.audio.source = AudioSource::Both;
    else if (audiosource == "none"||audiosource == "")
        settings.audio.source = AudioSource::None;


    //----------------------------------------------------
    // Source
    //----------------------------------------------------

    QString videosource = parser.value("videosource").toLower();

    if (videosource == "file")
    {
        settings.video.source = VideoSource::MediaFile;
        settings.video.fileName = parser.value("file");
    }
    else if (videosource == "camera")
    {
        settings.video.source = VideoSource::Camera;
        settings.video.device = parser.value("device");
    }
    else if (videosource == "screen")
    {
        settings.video.source = VideoSource::ScreenFFmpeg;
        //settings.video.source = CaptureSource::ScreenFFmpegPipeWire;
        settings.video.screenIndex =
                parser.value("screen").toInt();
    }
    else if (videosource == "none"||videosource=="")
    {
        settings.video.source = VideoSource::None;
    }

    //----------------------------------------------------
    // Network
    //----------------------------------------------------

    if (parser.isSet("address"))
    {
        settings.videoAddress =
                QHostAddress(parser.value("address"));

        settings.audioAddress =
                settings.videoAddress;
    }

    if (parser.isSet("video-port"))
    {
        settings.videoPort =
                parser.value("video-port").toUShort();

        settings.audioPort =
                settings.videoPort + 2;
    }

    return true;
}
