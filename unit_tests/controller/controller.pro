#-------------------------------------------------
#
# Project created by QtCreator 2012-03-30T11:12:25
#
#-------------------------------------------------

QT       += opengl gui testlib

TARGET = tst_controllertest
#CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG   += qt3d
DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA

SOURCES += tst_controllertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += ../../app/imagereader.h ../../app/defines.h \
           ../../app/controller.h ../../app/model3d.h \
           ../../app/modelball.h ../../app/markerdetector.h \
           ../../app/engine.h \
           ../../app/modeldisplay.h ../../app/languageselector.h

SOURCES += ../../app/controller.cpp ../../app/model3d.cpp \
           ../../app/modelball.cpp ../../app/markerdetector.cpp \
            ../../app/imagereader.cpp ../../app/engine.cpp \
            ../../app/modeldisplay.cpp \
            ../../app/languageselector.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"


INCLUDEPATH += /opt/local/include
INCLUDEPATH += ../../artoolkit/inc/
INCLUDEPATH += ../../app/



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
    LIBS += -L../../artoolkit/ -lartoolkit
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

    LIBS += -L../../../opencv-build/bin/Debug -lopencv_core231d \
            -lopencv_highgui231d -lopencv_imgproc231d

    LIBS += -L../../artoolkit/debug -lartoolkit
}
