/****************************************************************************
* AR Physics Teacher is an augmented reality teaching application
*
* Copyright (C) 2012-2014 University of Helsinki
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

#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <QObject>
#include <QList>
#include <opencv/cv.h>
#include <opencv/highgui.h>

/* A handle for OpenCV communication */
/* (Helps instrumentation in tests) */
typedef struct {
   CvCapture* (*cvCreateCameraCapture)(int deviceId);
   void (*cvReleaseCapture)(CvCapture **camera);
   int (*cvSetCaptureProperty)(CvCapture *camera, int propertyId, double value);
   IplImage* (*cvCreateImage)(CvSize size, int depth, int channels);
   IplImage* (*cvQueryFrame)(CvCapture *camera);
   void (*cvCvtColor)(const CvArr *src, CvArr *dest, int code);
   void (*cvFlip)(const CvArr *src, CvArr *dest, int code);
} opencv_handle;


/* Handles camera operations */
class ImageReader : public QObject
{
     Q_OBJECT
public:
    ImageReader(QObject *parent = 0);
    ~ImageReader();

    /* Opens a camera device and inits capture buffers */
    virtual bool initReader();

    /* Reads images from camera */
    virtual bool getImages(IplImage **rgbImage, IplImage **bgraImage);

public: // for the sake of testability

    /* Opens/closes a camera device */
    bool openCamera(int camera = -1);
    bool closeCamera();

    /* Updates available camera devices */
    bool listDevices(QList<int> &devices);

    /* Gives the next camera */
    bool nextCamera(QList<int> &devices, int *index);
    void nextCamera();
    opencv_handle *ir_ocv;

private:
    void setupOpenCv();
    CvCapture *_camera;
    int _cameraIndex;
    int _cameraNum;

    IplImage *_rgbImage;
    IplImage *_grayImage;
    IplImage *_bgraImage;
    QList<int> _devices;

};

#endif // IMAGEREADER_H
