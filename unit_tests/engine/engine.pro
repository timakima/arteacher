    #-------------------------------------------------
#
# Project created by QtCreator 2012-04-02T14:20:29
#
#-------------------------------------------------

QT       += opengl testlib

CONFIG += qt3d

TARGET = tst_enginetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += ../../app/engine.h ../../app/model3d.h ../../app/markerdetector.h \
           ../../app/defines.h
HEADERS += ../tool.h
SOURCES += tst_enginetest.cpp ../../app/engine.cpp ../../app/model3d.cpp \
            ../../app/markerdetector.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA

INCLUDEPATH += ../../artoolkit/inc/
INCLUDEPATH += ../../app/
INCLUDEPATH += ..

linux-g++* {
    LIBS += -L../../artoolkit/ -lartoolkit
}

# macports
mac* {
    INCLUDEPATH += /opt/local/include
    LIBS += -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
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
