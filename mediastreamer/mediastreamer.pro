QT += gui widgets core network multimedia dbus

CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += link_pkgconfig

TARGET=mediastreamer

PKGCONFIG += x264
PKGCONFIG += opus
PKGCONFIG += libpulse libpulse-simple

TEMPLATE = app

SOURCES += \
    audiocapture/audiomixer.cpp \
    audiocapture/microphoneaudiocapture.cpp \
    audiocapture/systemaudiocapture.cpp \
    converter/videoframeconverter.cpp \
    videocapture/ffmpegscreencapture.cpp \
    videocapture/mediafilecapture.cpp \
    videocapture/mediareaderthread.cpp \
    videocapture/pipewirescreencapture.cpp \
    videocapture/v4l2buffer.cpp \
    videocapture/v4l2capture.cpp \
    videocapture/waylandscreencast.cpp \
    common/camerainforeader.cpp \
    common/commandlineparser.cpp \
    common/mediafileinforeader.cpp \
    encoder/audioopusencoder.cpp \
    encoder/h264encoder.cpp \
    main.cpp \
    network/rtppacketizer.cpp \
    player/audioprocessor.cpp \
    sender/audiosender.cpp \
    sender/rtpsender.cpp \
    sender/videosender.cpp \
    session/httpserver.cpp \
    session/rtspconnection.cpp \
    session/rtspserver.cpp \
    session/sessiondescription.cpp \
    stream/senderpipeline.cpp \
    sync/mastertime.cpp \
    thread/capturethread.cpp \
    udpmesageservice/controlchannel.cpp \
    udpmesageservice/controlclient.cpp \
    udpmesageservice/controlmessage.cpp \
    udpmesageservice/controlserver.cpp


HEADERS += \
    audiocapture/audiomixer.h \
    audiocapture/microphoneaudiocapture.h \
    audiocapture/audioframe.h \
    audiocapture/systemaudiocapture.h \
    converter/videoframeconverter.h \
    videocapture/desktopsession.h \
    videocapture/ffmpegscreencapture.h \
    videocapture/mediafilecapture.h \
    videocapture/mediareaderthread.h \
    videocapture/pipewirescreencapture.h \
    videocapture/v4l2buffer.h \
    videocapture/v4l2capture.h \
    videocapture/waylandscreencast.h \
    common/EncodedPacket.h \
    common/MediaBuffer.h \
    common/MediaFrame.h \
    common/MediaPacket.h \
    common/StreamSettings.h \
    common/VideoBuffer.h \
    common/camerainforeader.h \
    common/commandlineparser.h \
    common/mediafileinforeader.h \
    common/streamenums.h \
    encoder/audioopusencoder.h \
    encoder/h264encoder.h \
    interfaces/ICapture.h \
    interfaces/IVideoCapture.h \
    network/rtppacket.h \
    network/rtppacketizer.h \
    player/audioprocessor.h \
    sender/audiosender.h \
    sender/rtpsender.h \
    sender/videosender.h \
    session/httpserver.h \
    session/rtspconnection.h \
    session/rtspserver.h \
    session/sessiondescription.h \
    stream/senderpipeline.h \
    sync/mastertime.h \
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
