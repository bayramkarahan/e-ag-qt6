#ifndef VIDEOCAPTURESETTINGS_H
#define VIDEOCAPTURESETTINGS_H

#include <QString>

enum class CaptureSource
{
    Camera,
    ScreenQt,
    ScreenFFmpeg,
    MediaFile,
    ScreenFFmpegPipeWire,
    ScreenWayland,
    ScreenPipeWire,
    ScreenX11
};

enum class TimeMode
{
    MasterClock,
    FramePts
};

struct VideoCaptureSettings
{
    // Kaynak tipi
    CaptureSource source = CaptureSource::Camera;
    TimeMode timeMode = TimeMode::MasterClock;
    //==================================================
    // Camera
    //==================================================

    QString device = "/dev/video0";

    //==================================================
    // Screen
    //==================================================

    int screenIndex = 0;
    QString windowId;

    //==================================================
    // Media File
    //==================================================

    QString fileName;
    bool loop = false;

    //==================================================
    // Network (RTSP/HTTP/RTMP vb.)
    //==================================================

    QString url;

    //==================================================
    // Ortak Ayarlar
    //==================================================

    int width  = 640;
    int height = 480;
    int fps    = 30;
};
#endif
