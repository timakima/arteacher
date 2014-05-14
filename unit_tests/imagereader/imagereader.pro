#-------------------------------------------------
#
# Project created by QtCreator 2012-02-17T10:30:51
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_imagereadertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += ../../app/
INCLUDEPATH += /opt/local/include


HEADERS += ../../app/imagereader.h ../../app/defines.h
SOURCES += tst_imagereadertest.cpp ../../app/imagereader.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


INCLUDEPATH += /opt/local/include



linux* {
    message("Building for Linux")
    INCLUDEPATH += /usr/include/opencv
    LIBS += -L../../artoolkit/ -lartoolkit
    LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
    #LIBS += -lcv -lhighgui -lcxcore -lcvaux
}

# macports
mac* {
    LIBS += -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
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

    LIBS += -L../../../opencv-build/bin/Debug -lopencv_core231d -lopencv_highgui231d -lopencv_imgproc231d

}
