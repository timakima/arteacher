#-------------------------------------------------
#
# Project created by QtCreator 2012-02-17T10:06:10
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG += qt3d

TARGET = areality
TEMPLATE = app

DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA
INCLUDEPATH += /opt/local/include
INCLUDEPATH += ../artoolkit/inc

linux* {
    message("Building for Linux")
    INCLUDEPATH += /usr/include/opencv
    LIBS += -L../artoolkit/ -lartoolkit
    LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
    #LIBS += -lcv -lhighgui -lcxcore -lcvaux
}

# macports
mac* {
    LIBS += -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
    LIBS += -L../artoolkit -lartoolkit
    LIBS += -framework Qt3D
}

# win
# expects opencv build in followin directories
win32* {
    INCLUDEPATH += ../../opencv
    INCLUDEPATH += ../../opencv/include
    INCLUDEPATH += ../../opencv/include/opencv
    INCLUDEPATH += ../../opencv/include/opencv2
    INCLUDEPATH += ../../opencv/modules/core/include
    INCLUDEPATH += ../../opencv/modules/imgproc/include
    INCLUDEPATH += ../../opencv/modules/video/include
    INCLUDEPATH += ../../opencv/modules/calib3d/include
    INCLUDEPATH += ../../opencv/modules/highgui/include
    INCLUDEPATH += ../../opencv/modules/features2d/include
    INCLUDEPATH += ../../opencv/modules/flann/include
    INCLUDEPATH += ../../opencv/modules/objdetect/include
    INCLUDEPATH += ../../opencv/modules/legacy/include

    LIBS += -L../artoolkit/release/ -lartoolkit
    LIBS += -L../../opencv-build/bin/Release -lopencv_core231 \
            -lopencv_highgui231 \
            -lopencv_imgproc231
}

SOURCES += main.cpp\
    imagereader.cpp \
    controller.cpp \
    viewportwidget.cpp \
    markerdetector.cpp \
    model3d.cpp \
    mainwindow.cpp \
    menubutton.cpp \
    languagebutton.cpp \
    camerabutton.cpp \
    modelball.cpp \
    engine.cpp \
    app.cpp \
    modeldisplay.cpp \
    boltzmanninfowidget.cpp \
    exitbutton.cpp \
    languageselector.cpp \
    lcdwidget.cpp

HEADERS  += \
    imagereader.h \
    controller.h \
    viewportwidget.h \
    markerdetector.h \
    defines.h \
    model3d.h \
    mainwindow.h \
    menubutton.h \
    languagebutton.h \
    camerabutton.h \
    modelball.h \
    engine.h \
    app.h \
    modeldisplay.h \
    boltzmanninfowidget.h \
    exitbutton.h \
    languageselector.h \
    lcdwidget.h

FORMS    +=

RESOURCES +=


