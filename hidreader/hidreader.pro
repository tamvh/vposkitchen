QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = hidreader
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    utils.cpp \
    menureader.cpp \
    commonfunction.cpp \
    encoder.cpp \
    hidapi/linux/hid.c \
    hidreadermain.cpp \
    ../common/udpbrdcastclient.cpp \
    ../common/wscssclient.cpp

HEADERS += \
    utils.h \
### hid device
    menureader.h \
    commonfunction.h \
    encoder.h \
    hidreadermain.h \
    ../common/udpbrdcastclient.h \
    ../common/wscssclient.h

LIBS += -L/usr/lib -ludev
