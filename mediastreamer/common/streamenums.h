#ifndef STREAMENUMS_H
#define STREAMENUMS_H
#pragma once

#include <QHostAddress>
#include <QString>

#include "../common/VideoCaptureSettings.h"

enum class StreamMode
{
    Sender,
    Receiver
};

enum class PreviewMode
{
    None,
    Local,
    Remote,
    Both
};

enum class CaptureMode
{
    None,
    Video,
    Audio,
    AudioVideo
};
#endif // STREAMENUMS_H
