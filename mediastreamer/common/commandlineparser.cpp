#include "commandlineparser.h"

#include <QCommandLineParser>
#include <QHostAddress>
#include <QDebug>

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
        "none | local | remote",
        "preview"
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
    else if (preview == "remote")
        settings.previewMode = PreviewMode::Remote;
    else if (preview == "local" || preview.isEmpty())
        settings.previewMode = PreviewMode::Local;

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
