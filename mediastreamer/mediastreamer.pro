QT += gui widgets core network multimedia dbus

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += link_pkgconfig

TARGET=mediastreamer

PKGCONFIG += x264
PKGCONFIG += opus

TEMPLATE = app

SOURCES += \
    audio/audiocapture.cpp \
    capture/ffmpegpipewirescreencapture.cpp \
    capture/ffmpegscreencapture.cpp \
    capture/mediafilecapture.cpp \
    capture/mediareaderthread.cpp \
    capture/pipewirescreencapture.cpp \
    capture/screencapture.cpp \
    capture/v4l2buffer.cpp \
    capture/v4l2capture.cpp \
    capture/waylandscreencast.cpp \
    common/camerainforeader.cpp \
    common/commandlineparser.cpp \
    common/mediafileinforeader.cpp \
    converter/videoconverter.cpp \
    converter/videoframeconverter.cpp \
    decoder/audioopusdecoder.cpp \
    decoder/h264decoder.cpp \
    decoder/h264depacketizer.cpp \
    encoder/audioopusencoder.cpp \
    encoder/h264encoder.cpp \
    main.cpp \
    network/rtpjitterbuffer.cpp \
    network/rtppacketizer.cpp \
    player/audioplayback.cpp \
    player/audioprocessor.cpp \
    preview/videopreview.cpp \
    receiver/rtpreceiver.cpp \
    renderer/videorenderer.cpp \
    sender/audiosender.cpp \
    sender/rtpsender.cpp \
    sender/videosender.cpp \
    session/httpserver.cpp \
    session/rtspconnection.cpp \
    session/rtspserver.cpp \
    session/sessiondescription.cpp \
    stream/mediastreamer.cpp \
    stream/receiverpipeline.cpp \
    stream/senderpipeline.cpp \
    sync/audioscheduler.cpp \
    sync/mastertime.cpp \
    sync/mediaclock.cpp \
    sync/videoscheduler.cpp \
    thread/capturethread.cpp \
    udpmesageservice/controlchannel.cpp \
    udpmesageservice/controlclient.cpp \
    udpmesageservice/controlmessage.cpp \
    udpmesageservice/controlserver.cpp


HEADERS += \
    audio/audiocapture.h \
    audio/audioframe.h \
    capture/ffmpegpipewirescreencapture.h \
    capture/ffmpegscreencapture.h \
    capture/mediafilecapture.h \
    capture/mediareaderthread.h \
    capture/pipewirescreencapture.h \
    capture/screencapture.h \
    capture/v4l2buffer.h \
    capture/v4l2capture.h \
    capture/waylandscreencast.h \
    common/EncodedPacket.h \
    common/MediaBuffer.h \
    common/MediaClock.h \
    common/MediaFrame.h \
    common/MediaPacket.h \
    common/StreamSettings.h \
    common/VideoBuffer.h \
    common/VideoCaptureSettings.h \
    common/camerainforeader.h \
    common/commandlineparser.h \
    common/mediafileinforeader.h \
    common/streamenums.h \
    converter/videoconverter.h \
    converter/videoframeconverter.h \
    decoder/audioopusdecoder.h \
    decoder/h264decoder.h \
    decoder/h264depacketizer.h \
    encoder/audioopusencoder.h \
    encoder/h264encoder.h \
    interfaces/ICapture.h \
    interfaces/IVideoCapture.h \
    network/rtpjitterbuffer.h \
    network/rtppacket.h \
    network/rtppacketizer.h \
    player/audioplayback.h \
    player/audioprocessor.h \
    preview/videopreview.h \
    receiver/rtpreceiver.h \
    renderer/videorenderer.h \
    sender/audiosender.h \
    sender/rtpsender.h \
    sender/videosender.h \
    session/httpserver.h \
    session/rtspconnection.h \
    session/rtspserver.h \
    session/sessiondescription.h \
    stream/mediastreamer.h \
    stream/receiverpipeline.h \
    stream/senderpipeline.h \
    sync/audioscheduler.h \
    sync/mastertime.h \
    sync/mediaclock.h \
    sync/videoscheduler.h \
    thread/capturethread.h \
    udpmesageservice/controlchannel.h \
    udpmesageservice/controlclient.h \
    udpmesageservice/controlmessage.h \
    udpmesageservice/controlserver.h

INCLUDEPATH += \
    /usr/include \
    /usr/include/x86_64-linux-gnu

LIBS += \
    -lavdevice \
    -lavformat \
    -lavcodec \
    -lavutil \
    -lswscale \
    -lswresample
    #INCLUDEPATH += /usr/include/pipewire-0.3
    LIBS += -lpipewire-0.3
    PKGCONFIG += libpipewire-0.3

    INCLUDEPATH += \
        /usr/include/pipewire-0.3 \
        /usr/include/spa-0.2

target.path = /usr/bin
INSTALLS += target
