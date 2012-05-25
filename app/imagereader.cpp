/****************************************************************************
* AR Physics Teacher is an augmented reality teaching application
*
* Copyright (C) 2012 University of Helsinki
*
* Contact: Timo Makimattila <timo.makimattila@primoceler.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
* USA.
*
****************************************************************************/

#include <QDebug>
#include "imagereader.h"
#include "defines.h"

ImageReader::ImageReader():
    ir_ocv(new opencv_handle),
    _camera(NULL), _cameraIndex(0)

{
    setupOpenCv();
}


ImageReader::~ImageReader()
{
    closeCamera();
    delete ir_ocv;
}

void ImageReader::setupOpenCv() {
    ir_ocv->cvCreateCameraCapture = &cvCreateCameraCapture;
    ir_ocv->cvReleaseCapture = &cvReleaseCapture;
    ir_ocv->cvSetCaptureProperty = &cvSetCaptureProperty;
    ir_ocv->cvCreateImage = &cvCreateImage;
    ir_ocv->cvQueryFrame = &cvQueryFrame;
    ir_ocv->cvCvtColor = &cvCvtColor;
    ir_ocv->cvFlip = &cvFlip;
}

bool ImageReader::initReader() {
    bool ret;
    ret = listDevices(_devices);
    if (_devices.size()) {
        openCamera(_devices.at(0));
    }
    return ret;
}

/* public */
bool ImageReader::getImages(IplImage **rgbImage, IplImage **bgraImage) {

    bool ret;
    IplImage *rawImage;

    if (!_camera) {
        goto error;
    }

    rawImage = ir_ocv->cvQueryFrame(_camera);
    if (!rawImage) {
        goto error;
    }

    ir_ocv->cvCvtColor(rawImage, _rgbImage, CV_BGR2RGB);
    ir_ocv->cvFlip(_rgbImage, __null, 0);
    ir_ocv->cvCvtColor(_rgbImage, _grayImage, CV_BGR2GRAY);
    ir_ocv->cvCvtColor(_grayImage, _bgraImage, CV_GRAY2BGRA);

    *rgbImage = _rgbImage;
    ir_ocv->cvFlip(_bgraImage, __null, 0);
    *bgraImage = _bgraImage;

    ret = true;
    goto done;

    error:
    ret = false;

    done:
    return ret;
}


/* private */
bool ImageReader::openCamera(int camera) {
    qDebug() << "Opening camera device " << camera;
    bool ret;
    IplImage *image;
    CvSize size;
    _camera = ir_ocv->cvCreateCameraCapture(camera);
    if (!_camera) {
        qDebug() << "Unable to open camera";
        goto error;
    }


    ir_ocv->cvSetCaptureProperty(_camera, CV_CAP_PROP_FRAME_WIDTH, SCREEN_WIDTH);
#ifndef Q_OS_LINUX
    ir_ocv->cvSetCaptureProperty(_camera, CV_CAP_PROP_FRAME_HEIGHT, SCREEN_HEIGHT);
#endif
    image = ir_ocv->cvQueryFrame(_camera);
    //if (image) qDebug() << "got image " << image->width << image->height;

    //size = cvGetSize(image);
    size.width = SCREEN_WIDTH;
    size.height = SCREEN_HEIGHT;

    _grayImage = ir_ocv->cvCreateImage(size, image->depth, 1);
    _grayImage->origin = image->origin;

    _bgraImage = ir_ocv->cvCreateImage(size, 8, 4);
    _bgraImage->origin = image->origin;

    _rgbImage = ir_ocv->cvCreateImage(size, 8, 3);
    _rgbImage->origin = image->origin;

    ret = true;
    goto done;

    error:
    ret = false;

    done:
    return ret;
}

bool ImageReader::closeCamera() {
    ir_ocv->cvReleaseCapture(&_camera);
    _camera = NULL;
    return true;
}

bool ImageReader::listDevices(QList<int> &devices) {
    int max;
    bool ret;
    max = MAX_CAMERA_DEVICES;

    for (int deviceId = 0; deviceId < max; deviceId++) {
        if (openCamera(deviceId)) {
            devices.append(deviceId);
            closeCamera();
        }
    }

    qDebug() << "Camera devices:" << devices;
    if (devices.count() == 0) {
        ret = false;
    } else {
        ret = true;
    }

    return ret;
}

bool ImageReader::nextCamera(QList<int> &devices, int *index) {
    int deviceMax = devices.size();
    int deviceNum;

    /* Only one camera */
    if (deviceMax < 2) {
        goto done;
    }

    closeCamera();
    *index = *index + 1;
    *index = *index % deviceMax;
    deviceNum = devices.at(*index);
    openCamera(deviceNum);

    done:
    return true;
}

void ImageReader::nextCamera() {
    nextCamera(_devices, &_cameraIndex);
}

