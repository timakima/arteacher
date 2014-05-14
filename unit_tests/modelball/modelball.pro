#-------------------------------------------------
#
# Project created by QtCreator 2012-03-27T12:44:27
#
#-------------------------------------------------

QT       += gui opengl testlib

TARGET = tst_modelballtest
CONFIG   += console qt3d
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_modelballtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA

TEMPLATE = app

INCLUDEPATH += ../../artoolkit/inc/
INCLUDEPATH += ../../app/ ../
HEADERS += ../../app/defines.h ../../app/model3d.h ../../app/modelball.h
HEADERS += ../tool.h
SOURCES += ../../app/model3d.cpp ../../app/modelball.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

linux-g++* {
    LIBS += -L../../artoolkit/ -lartoolkit
}

# macports
mac* {
    INCLUDEPATH += /opt/local/include
    LIBS += -L../../artoolkit/ -lartoolkit
    LIBS += -framework Qt3D
}

# win
win32* {
    INCLUDEPATH += ../../../opencv
    INCLUDEPATH += ../../../opencv/include
    INCLUDEPATH += ../../../opencv/include/opencv
    INCLUDEPATH += ../../../opencv/include/opencv2
    INCLUDEPATH += ../../../opencv/modules/core/include
    INCLUDEPATH += ../../../opencv/modules/imgproc/include
    INCLUDEPATH += ../../../opencv/modules/video/include
    INCLUDEPATH += ../../../opencv/modules/calib3d/include
    INCLUDEPATH += ../../../opencv/modules/highgui/include
    INCLUDEPATH += ../../../opencv/modules/features2d/include
    INCLUDEPATH += ../../../opencv/modules/flann/include
    INCLUDEPATH += ../../../opencv/modules/objdetect/include
    INCLUDEPATH += ../../../opencv/modules/legacy/include
    LIBS += -L../../artoolkit/debug/ -lartoolkit

}
