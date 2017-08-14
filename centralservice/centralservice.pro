QT       += core websockets
QT       -= gui

CONFIG += c++11 gui concurrent svg network websockets printsupport bluetooth sql

TARGET = centralservice
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    clientinfo.cpp \
    udpbrdcastserver.cpp \
    servermanager.cpp \
    wsclient.cpp \
    http/httpbase.cpp \
    http/httpparams.cpp \
    http/httptool.cpp \
    http/json.cpp \
    http/logger.cpp \
    downloadfileman.cpp \
    commonfunction.cpp

HEADERS += \
    server.h \
    clientinfo.h \
    udpbrdcastserver.h \
    servermanager.h \
    wsclient.h \
    http/httpbase.h \
    http/httpparams.h \
    http/httptool.h \
    http/json.h \
    http/logger.h \
    downloadfileman.h

SUBDIRS += \
    centralservice.pro
