#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T15:58:42
#
#-------------------------------------------------

QT       -= gui

TARGET = artoolkit
TEMPLATE = lib

DEFINES += ARTOOLKITLIBRARY
DEFINES += AR_DEFAULT_PIXEL_FORMAT=AR_PIXEL_FORMAT_BGRA

SOURCES += src/arDetectMarker.c \
 src/arDetectMarker2.c \
 src/arGetCode.c \
 src/arGetMarkerInfo.c \
 src/arGetTransMat.c \
 src/arGetTransMat2.c \
 src/arGetTransMat3.c \
 src/arGetTransMatCont.c \
 src/arLabeling.c \
 src/arUtil.c \
 src/mAlloc.c \
 src/mAllocDup.c \
 src/mAllocInv.c \
 src/mAllocMul.c \
 src/mAllocTrans.c \
 src/mAllocUnit.c \
 src/mDet.c \
 src/mDisp.c \
 src/mDup.c \
 src/mFree.c \
 src/mInv.c \
 src/mMul.c \
 src/mPCA.c \
 src/mSelfInv.c \
 src/mTrans.c \
 src/mUnit.c \
 src/paramChangeSize.c \
 src/paramDecomp.c \
 src/paramDisp.c \
 src/paramDistortion.c \
 src/paramFile.c \
 src/paramGet.c \
 src/vAlloc.c \
 src/vDisp.c \
 src/vFree.c \
 src/vHouse.c \
 src/vInnerP.c \
 src/vTridiag.c

HEADERS += inc/AR/config.h \
        inc/AR/matrix.h \
        inc/AR/param.h \
        inc/AR/ar.h \
        inc/AR/arMulti.h \
        inc/AR/arvrml.h


INCLUDEPATH += inc


OTHER_FILES += \
    ../README.txt
