#-------------------------------------------------
#
# Project created by QtCreator 2012-03-20T16:45:27
#
#-------------------------------------------------

QT       += testlib gui

TARGET = tst_model3dtest
CONFIG   += console
CONFIG   += qt3d
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA

TEMPLATE = app

INCLUDEPATH += ../../artoolkit/inc/
INCLUDEPATH += ../../app/
HEADERS += ../../app/defines.h ../../app/model3d.h
SOURCES += tst_model3dtest.cpp ../../app/model3d.cpp
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
