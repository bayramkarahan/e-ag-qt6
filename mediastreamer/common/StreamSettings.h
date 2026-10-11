#pragma once

#include "streamenums.h"

struct AudioCaptureSettings
{
       AudioSource source = AudioSource::Microphone;
       int bitrate = 64000;
       int sampleRate = 48000;
       int frameSample = 960;
       int channels = 2;
};

struct VideoCaptureSettings
{
    // Kaynak tipi
    VideoSource source = VideoSource::Camera;
    TimeMode timeMode = TimeMode::MasterClock;
    int bitrate = 4000;

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


struct StreamSettings
{
    //----------------------------------------------------
    // Genel
    //----------------------------------------------------

    StreamMode mode = StreamMode::Sender;
    SenderMode sendermode = SenderMode::Unicast;

    VideoCaptureSettings video;
    AudioCaptureSettings audio;

    //----------------------------------------------------
    // Preview
    //----------------------------------------------------

    PreviewMode previewMode = PreviewMode::Local;

    //----------------------------------------------------
    // Network
    //----------------------------------------------------

    QHostAddress videoAddress = QHostAddress("239.10.10.10");
    quint16 videoPort = 5004;

    QHostAddress audioAddress = videoAddress;
    quint16 audioPort = videoPort+2;

    //----------------------------------------------------
    // Codec
    //----------------------------------------------------



    int multicastTtl = 1;

    QString networkInterface;

};

