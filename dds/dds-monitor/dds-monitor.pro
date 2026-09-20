QT       += core gui network concurrent
TARGET = dds-monitor
TEMPLATE = app
#LIBS += -lz
#CONFIG += console c++17

CONFIG += c++17
LIBS += -lnorm
LIBS += -lprotokit
LIBS += -lpthread

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    monitorclient.cpp \
    roledialog.cpp \
    userprivilegehelper.cpp

HEADERS += \
    Database.h \
    buttonwidget.h \
    listeWidget.h \
    monitorclient.h \
    pathwidget.h \
    roledialog.h \
    type.h \
    udpservice.h \
    update.h \
    userprivilegehelper.h


target.path = /usr/bin

nemomenu.files = data/nemomenu/*
nemomenu.path = /usr/share/nemo/actions/

nautilusmenu.files = data/nautilusmenu/*
nautilusmenu.path = /usr/share/nautilus/scripts/

dolphinmenu.files = data/dolphinmenu/*
dolphinmenu.path = /usr/share/kservices5/ServiceMenus/

thunarmenu.files = data/thunarmenu/*
thunarmenu.path = /usr/share/Thunar/

auto_start.files = data/dds.desktop
auto_start.path = /etc/xdg/autostart/

desktop_file.files = data/ddsapp.desktop
desktop_file.path = /usr/share/applications/

icon.files = icons/dds.svg
icon.path = /usr/share/icons/

ddsjson.files = data/dds.json
ddsjson.path = /usr/share/dds/

INSTALLS += target nemomenu nautilusmenu \
dolphinmenu thunarmenu auto_start ddsjson desktop_file icon

RESOURCES += \
    icons.qrc


