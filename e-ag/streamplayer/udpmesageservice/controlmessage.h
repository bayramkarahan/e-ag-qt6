#pragma once

#include <QByteArray>
#include <QDataStream>


enum class ControlMessageType : quint16
{
    Invalid = 0,

    // Discovery
    WhoIsServer = 1,
    IAmServer,

    // Connection
    Hello,
    HelloAck,

    // Stream information
    GetStreamInfo,
    StreamInfo,

    // Stream control
    StartStream,
    StartStreamFFmpegScreenCaptureAudio,
    StartStreamFFmpegScreenCaptureVideo,
    StartStreamFFmpegScreenCaptureAudioVideo,
    StopStream,
    PauseStream,
    ResumeStream,

    // Connection test
    Ping,
    Pong,

    // Generic data
    Data,

    // Error
    Error
};


struct ControlMessage
{
    ControlMessageType type =
        ControlMessageType::Invalid;

    quint32 sequence = 0;

    quint32 senderId = 0;

    QByteArray payload;
};


/*
 * ControlMessage -> QByteArray
 */
QByteArray serializeControlMessage(
    const ControlMessage &message);


/*
 * QByteArray -> ControlMessage
 */
bool deserializeControlMessage(
    const QByteArray &data,
    ControlMessage &message);


