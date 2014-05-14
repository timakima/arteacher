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

#include "imagereader.h"

#include <QtCore/QString>
#include <QtTest/QtTest>


CvCapture* fake_cvCreateCameraCapture(int deviceId) {
    void *capture = malloc(1);
    return (CvCapture*)capture;
}

void fake_cvReleaseCapture(CvCapture **camera) {
}

int fake_cvSetCaptureProperty(CvCapture *camera, int propertyId, double value) {
    return 0;
}

IplImage* fake_cvCreateImage(CvSize size, int depth, int channels) {
    IplImage *image = (IplImage*)malloc((size.width + size.height) * 3);
    return image;
}

IplImage* fake_cvQueryFrame(CvCapture *camera) {
    IplImage *image = (IplImage*)malloc(640*480 * 3);
    return image;
}

void fake_cvCvtColor(const CvArr *src, CvArr *dest, int code) {
}

void fake_cvFlip(const CvArr *src, CvArr *dest, int code) {
}

class ImagereaderTest : public QObject
{
    Q_OBJECT
    
public:
    ImagereaderTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void openCamera();
    void getImages();
    void closeCamera();
    void listDevices();
    void nextCamera();
private:
    ImageReader *_reader;

};

ImagereaderTest::ImagereaderTest()
{
}

void ImagereaderTest::initTestCase() {
    QVERIFY(_reader = new ImageReader());

    /* Fake OpenCV backend */
    _reader->ir_ocv->cvCreateCameraCapture = &fake_cvCreateCameraCapture;
    _reader->ir_ocv->cvReleaseCapture = &fake_cvReleaseCapture;
    _reader->ir_ocv->cvSetCaptureProperty = &fake_cvSetCaptureProperty;
    _reader->ir_ocv->cvCreateImage = &fake_cvCreateImage;
    _reader->ir_ocv->cvQueryFrame = &fake_cvQueryFrame;
    _reader->ir_ocv->cvCvtColor = &fake_cvCvtColor;
    _reader->ir_ocv->cvFlip = &fake_cvFlip;
    _reader->initReader();

}


void ImagereaderTest::cleanupTestCase() {
    delete _reader;
}

void ImagereaderTest::openCamera()
{
    QVERIFY(_reader->openCamera(-1));
}

void ImagereaderTest::getImages()
{
    IplImage *rgbImage;
    IplImage *bgraImage;
    QVERIFY(_reader->getImages(&rgbImage, &bgraImage));
    QVERIFY(rgbImage);
    QVERIFY(bgraImage);
}


void ImagereaderTest::closeCamera()
{
    QVERIFY(_reader->closeCamera());
}

void ImagereaderTest::listDevices() {
    QList<int> devices;

    _reader->listDevices(devices);
    QListIterator<int> i(devices);
    while(i.hasNext()) {
        qDebug() << "Found device " << i.next();
    }
}

void ImagereaderTest::nextCamera() {
    QList<int> devices;
    _reader->listDevices(devices);
    QListIterator<int> i(devices);
    int index = 0;

    _reader->openCamera(0);
    while(i.hasNext()) {
        _reader->nextCamera(devices, &index);
        i.next();
    }

    /* Should be the first one */
    QVERIFY(index == 0);
    _reader->nextCamera(devices, &index);
    QVERIFY(index == 1);
}

QTEST_APPLESS_MAIN(ImagereaderTest)

#include "tst_imagereadertest.moc"
