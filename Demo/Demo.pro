#-------------------------------------------------
#
# Project created by QtCreator 2015-08-23T20:04:46
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demo
TEMPLATE = app


SOURCES += main.cpp\
        chatclient.cpp \
    unicast.cpp

HEADERS  += chatclient.h \
    unicast.h

FORMS    += chatclient.ui \
    unicast.ui
