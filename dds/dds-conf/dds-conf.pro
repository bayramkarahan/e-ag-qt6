QT += core gui network concurrent
TARGET = dds-conf
TEMPLATE = app
CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp
HEADERS += \
    Database.h
target.path = /usr/bin

INSTALLS += target

RESOURCES +=
