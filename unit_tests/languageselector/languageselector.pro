#-------------------------------------------------
#
# Project created by QtCreator 2012-04-26T12:58:43
#
#-------------------------------------------------

QT       += testlib

#QT       -= gui

TARGET = tst_languageselectortest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += ../tool.h
INCLUDEPATH += ../ ../../app
HEADERS += ../../app/languageselector.h
SOURCES += tst_languageselectortest.cpp ../../app/languageselector.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
