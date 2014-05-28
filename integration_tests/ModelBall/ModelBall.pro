#-------------------------------------------------
#
# Project created by QtCreator 2014-05-29T20:07:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModelBall
TEMPLATE = app

DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA


QT       += core gui opengl testlib

CONFIG += qt3d

SOURCES += main.cpp\
        mainwindow.cpp \
        ../../app/modelball.cpp ../../app/model3d.cpp \
    view.cpp

HEADERS  += mainwindow.h ../../app/modelball.h ../../app/model3d.h \
    ../../app/defines.h   view.h

INCLUDEPATH += ../../app/ ../../artoolkit/inc

LIBS += -L../../artoolkit -lartoolkit

mac* {
    LIBS += -framework Qt3D
}
