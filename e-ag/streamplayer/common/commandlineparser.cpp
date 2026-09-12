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
        "source",
        "file | camera | screen",
        "source"
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
        "capture",
        "audio | video | av",
        "capture"
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
    // Capture Mode
    //----------------------------------------------------

    QString capture = parser.value("capture").toLower();

    if (capture == "audio")
        settings.captureMode = CaptureMode::Audio;
    else if (capture == "video")
        settings.captureMode = CaptureMode::Video;
    else if (capture == "av" || capture.isEmpty())
        settings.captureMode = CaptureMode::AudioVideo;

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

    QString source = parser.value("source").toLower();

    if (source == "file")
    {
        settings.video.source = CaptureSource::MediaFile;
        settings.video.fileName = parser.value("file");
    }
    else if (source == "camera")
    {
        settings.video.source = CaptureSource::Camera;
        settings.video.device = parser.value("device");
    }
    else if (source == "screen")
    {
        settings.video.source = CaptureSource::ScreenFFmpeg;
        settings.video.screenIndex =
                parser.value("screen").toInt();
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
