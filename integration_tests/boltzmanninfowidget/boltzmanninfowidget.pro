#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T08:14:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = boltzmanninfowidget
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp ../../app/boltzmanninfowidget.cpp


INCLUDEPATH += ../../app
HEADERS  += widget.h ../../app/boltzmanninfowidget.h \
    ../../app/defines.h
