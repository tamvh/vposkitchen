QT       += core websockets
QT       -= gui

CONFIG += c++11

TARGET = centralservice
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    clientinfo.cpp \
    udpbrdcastserver.cpp

HEADERS += \
    server.h \
    clientinfo.h \
    udpbrdcastserver.h
