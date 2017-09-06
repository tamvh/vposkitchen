QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = webnotify
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    utils.cpp \
    commonfunction.cpp \
    ../common/udpbrdcastclient.cpp \
    ../common/wscssclient.cpp \
    recievenotify.cpp \
    wsclient.cpp \
    http/httpbase.cpp \
    http/httpbase2.cpp \
    http/httpparams.cpp \
    http/httptool.cpp \
    http/json.cpp \
    http/logger.cpp \
    ../common/prntdataformat.cpp \
    cmn.cpp

HEADERS += \
    utils.h \
    commonfunction.h \
    ../common/udpbrdcastclient.h \
    ../common/wscssclient.h \
    recievenotify.h \
    wsclient.h \
    http/httpbase.h \
    http/httpparams.h \
    http/httptool.h \
    http/json.h \
    http/logger.h \
    http/httpbase2.h \
    ../common/prntdataformat.h \
    cmn.h
