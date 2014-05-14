#-------------------------------------------------
#
# Project created by QtCreator 2012-02-24T14:52:51
#
#-------------------------------------------------

QT       += testlib gui


TARGET = tst_markerdetectortest
CONFIG   += console
CONFIG   += qt3d

CONFIG   -= app_bundle

DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA

TEMPLATE = app

INCLUDEPATH += ../../artoolkit/inc/
INCLUDEPATH += ../../app/
HEADERS += ../../app/markerdetector.h ../../app/defines.h ../../app/model3d.h
SOURCES += tst_markerdetectortest.cpp ../../app/markerdetector.cpp \
            ../../app/model3d.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


linux* {
    message("Building for Linux")
    INCLUDEPATH += /usr/include/opencv
    LIBS += -L../../artoolkit/ -lartoolkit
    LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
    #LIBS += -lcv -lhighgui -lcxcore -lcvaux
}

# macports
mac* {
    INCLUDEPATH += /opt/local/include
    LIBS += -L../../artoolkit/ -lartoolkit
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
    LIBS += -L../../artoolkit/debug/ -lartoolkit
    LIBS += -L../../../opencv-build/bin/Debug -lopencv_core231d -lopencv_highgui231d \
            -lopencv_imgproc231d

}
