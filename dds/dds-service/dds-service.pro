QT += core network concurrent
TARGET = dds-service
TEMPLATE = app
CONFIG += c++17
LIBS += -lnorm

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    discoverymanager.cpp \
    filereceiver.cpp \
    filesender.cpp \
    main.cpp \
    monitorserver.cpp \
    normbandwidthestimator.cpp \
    normnetworkprobe.cpp \
    normreceiver.cpp \
    normreceiverworker.cpp \
    normsender.cpp \
    normsenderworker.cpp \
    transfermanager.cpp \
    udppacketrouter.cpp \
    ddsservice.cpp \
    userprivilegehelper.cpp
HEADERS += \
    Database.h \
    discoverymanager.h \
    filereceiver.h \
    filesender.h \
    monitorserver.h \
    normbandwidthestimator.h \
    normnetworkprobe.h \
    normreceiver.h \
    normreceiverworker.h \
    normsender.h \
    normsenderworker.h \
    transfermanager.h \
    type.h \
    udppacketrouter.h \
    ddsservice.h \
    userprivilegehelper.h

target.path = /usr/bin

service.files = data/dds-service.service
service.path = /lib/systemd/system/

debscriptservice.files = data/dds-debinstaller@.service
debscriptservice.path = /lib/systemd/system/

debscript.files = data/dds-scriptexec
debscript.path = /usr/share/dds/

scriptscriptservice.files = data/dds-scriptexec@.service
scriptscriptservice.path = /lib/systemd/system/

scriptscript.files = data/dds-debinstaller
scriptscript.path = /usr/share/dds/

INSTALLS += target service \
scriptscript scriptscriptservice debscript debscriptservice
