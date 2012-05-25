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


#include <QtCore/QString>
#include <QtGui/QApplication>
#include <QtTest/QtTest>
#include "controller.h"



/* Fake image reader without real camera */
class FakeImageReader : public ImageReader {
public:
    FakeImageReader(bool fail) { _fail = fail; }
    bool initReader() { return _fail; }
    bool getImages(IplImage **rgbImage, IplImage **bgraImage) {
        CvSize size;
        size.width = SCREEN_WIDTH;
        size.height = SCREEN_HEIGHT;
        IplImage *gray;
        IplImage *bgra;

        IplImage *raw =
                cvLoadImage("tests/testdata/bgra_image_with_marker.png");
        gray =  cvCreateImage(size, raw->depth, 1);
        gray->origin = raw->origin;

        bgra = cvCreateImage(size, 8, 4);
        bgra->origin = raw->origin;

        cvCvtColor(raw, gray, CV_BGR2GRAY);
        cvCvtColor(gray, bgra, CV_GRAY2BGRA);

        *rgbImage = raw;
        *bgraImage = bgra;

        return _fail;
    }
    bool _fail;
};


class ControllerTest : public QObject
{
    Q_OBJECT
    
public:
    ControllerTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void createModel();
    void loadConfig();
    void initAR();
private:
    ImageReader *_fake;
    Controller *_controller;
};

ControllerTest::ControllerTest()
{
}

void ControllerTest::initTestCase() {
    _fake = new FakeImageReader(true);
    _controller = new Controller(0, _fake, 0);
}

void ControllerTest::cleanupTestCase() {
    delete _fake;
    delete _controller;
}

void ControllerTest::createModel()
{
    QString modelFile("../models/thermo.3ds");
    QString patternFile("../markers/marker_a.pat");
    qreal angle = 30;
    QVector3D rot(1, 0, 1);
    Model3D *model;

    bool ret = false;

    ret = _controller->createModel(modelFile,
                     patternFile,
                     angle,
                     rot,
                     &model);

    QVERIFY(ret == true);
    QVERIFY(model != NULL);
}

void ControllerTest::loadConfig() {
    int argc = 1;
    char *argv = "foobar";
    QApplication a(argc, &argv);

    QString configDir("../");
    QString testDir("tests/");

    /* Verify that loadConfig finds a scene (name and image for a button) */
    QSignalSpy spy(_controller, SIGNAL(newButton(QString&,QString&)));
    QVERIFY(_controller->_models.size() == 0);
    QDir::setCurrent(configDir);
    QVERIFY(_controller->loadConfig() == true);
    QDir::setCurrent(testDir);
    QVERIFY(_controller->_models.size() > 0);
    QVERIFY(spy.count() > 0);
}

void ControllerTest::initAR() {

    int argc = 1;
    char *argv = "foobar";
    QApplication a(argc, &argv);

    QString configDir("../");
    QString testDir("tests/");
    QDir::setCurrent(configDir);
    QVERIFY(_controller->initAR() == true);
    _controller->shutdownAR();

    /* test failure */
    delete _controller;
    delete _fake;
    _fake = new FakeImageReader(false);
    _controller = new Controller(0, _fake);
    QVERIFY(_controller->initAR() == false);
    _controller->shutdownAR();
    QDir::setCurrent(testDir);
}


QTEST_APPLESS_MAIN(ControllerTest)

#include "tst_controllertest.moc"
