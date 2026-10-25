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
    NormalPreview,
    FullScreenPreview,
    MultiPreview
};


enum class VideoSource
{
    None,
    Camera,
    ScreenFFmpeg,
    MediaFile,
    ScreenPipeWire

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
