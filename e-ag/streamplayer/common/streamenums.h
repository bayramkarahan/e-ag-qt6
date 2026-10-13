#ifndef STREAMENUMS_H
#define STREAMENUMS_H
#pragma once

#include <QHostAddress>
#include <QString>

enum class TimeMode
{
    MasterClock,
    FramePts
};

enum class StreamMode
{
    Sender,
    Receiver
};
enum class ReceiverMode
{
    Unicast,
    Multicast
};

enum class PreviewMode
{
    None,
    Normal,
    FullScreen
};

enum class VideoSource
{
    None,
    Camera,
    ScreenQt,
    ScreenFFmpeg,
    MediaFile,
    ScreenFFmpegPipeWire,
    ScreenWayland,
    ScreenPipeWire,
    ScreenX11
};

enum class AudioSource
{
    None,
    Microphone,
    System,
    Both,
    MediaFile
};

#endif // STREAMENUMS_H
