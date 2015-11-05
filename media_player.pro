#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T21:31:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Media-Player
TEMPLATE = app


SOURCES += gui.cpp \
    Application.cpp \
    AudioData.cpp \
    coder.cpp \
    audiocontroller.cpp \
    audioplayer.cpp \
    channelwave.cpp

HEADERS  += gui.h \
    AudioData.h \
    coder.h \
    audiocontroller.h \
    audioplayer.h \
    channelwave.h

FORMS    += gui.ui
