#include "commandlineparser.h"

#include <QCommandLineParser>
#include <QHostAddress>
#include <QDebug>

bool CommandLineParser::parse(QApplication &app,
                              StreamSettings &settings)
{
    QCommandLineParser parser;

    parser.setApplicationDescription("streamplayer");
    parser.addHelpOption();
    parser.addVersionOption();


    parser.addOption({
        "receivermode",
        "unicast | multicast",
        "receivermode"
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
        "preview",
        "none | normal | multi | fullscreen",
        "preview"
    });

    parser.addOption({
        "address",
        "Multicast address",
        "address"
    });

    parser.addOption({
           "serveraddress",
           "local | server",
           "address"
       });

    parser.addOption({
        "video-port",
        "Video RTP port",
        "port"
    });

    parser.process(app);

    //----------------------------------------------------
    // ReceiverMode
    //----------------------------------------------------

    QString receivermode = parser.value("receivermode").toLower();

    if (receivermode == "unicast")
        settings.receivermode = ReceiverMode::Unicast;
    else if (receivermode == "multicast")
        settings.receivermode = ReceiverMode::Multicast;
    else
    {
        //qCritical() << "set receivermode Multicast";
        settings.receivermode = ReceiverMode::Multicast;
        ///parser.showHelp(1);
    }

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
    // Preview
    //----------------------------------------------------

    QString preview = parser.value("preview").toLower();

    if (preview == "none")
        settings.previewMode = PreviewMode::None;
    else if (preview == "fullscreen")
        settings.previewMode = PreviewMode::FullScreenPreview;
    else if (preview == "multi")
        settings.previewMode = PreviewMode::MultiPreview;
    else if (preview == "normal" || preview.isEmpty())
        settings.previewMode = PreviewMode::NormalPreview;
    //----------------------------------------------------
    // videosource
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
    // Server Address
    //----------------------------------------------------

    QString serverAddress = parser.value("serveraddress").trimmed();

    if (serverAddress.isEmpty() ||
        serverAddress.toLower() == "local")
    {
        settings.serverAddress ="127.0.0.1";
    }
    else
    {
        QHostAddress address(serverAddress);

        if (address.isNull())
        {
            qCritical() << "Invalid server address:"
                        << serverAddress;

            return false;
        }

        settings.serverAddress = serverAddress;
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
