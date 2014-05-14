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
#include <QtTest/QtTest>
#include <QDir>
#include <QPixmap>

#include "markerdetector.h"
#include "defines.h"

class MarkerdetectorTest : public QObject
{
    Q_OBJECT
    
public:
    MarkerdetectorTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void setParameters();
    void getMarker();
private:
    Model3D *_banana;
    Model3D *_owl;
    MarkerDetector *_detector;
    int _bananaId;
    int _owlId;

};

MarkerdetectorTest::MarkerdetectorTest()
{
}


void MarkerdetectorTest::initTestCase() {
    _detector = new MarkerDetector();
    QVERIFY(_detector);
    _banana = new Model3D();
    QVERIFY(_banana);
    _banana->addModel("models/blank.3ds", "markers/patt.hiro");

    _owl = new Model3D();
    _owl->addModel("models/blank.3ds", "markers/patt.hiro");
}

void MarkerdetectorTest::cleanupTestCase() {
    delete _detector;
}

void MarkerdetectorTest::setParameters() {
    QString cameraFile = "camera.dat";
    _detector->setParameters(SCREEN_WIDTH, SCREEN_HEIGHT, 100,
                             cameraFile);
}


void MarkerdetectorTest::getMarker() {

    coordinates_t *coords;
    double expected_position[8];
    QList<Model3D*> models;

    /* Should be seen */
    models.append(_banana);

    /* Should not be seen */
    models.append(_owl);

    QImage hasMarker("tests/testdata/bgra_image_with_marker.png");
    QImage noMarker("tests/testdata/bgra_image_without_marker.png");

    coords = _banana->coordPtr(_bananaId);
    memset(expected_position, 0, 8 * sizeof(double));

    /* No marker, position zero */
    QVERIFY(!_detector->getMarker(NULL, &models));
    QVERIFY(!_detector->getMarker(noMarker.bits(), &models));

    /* Marker found, position changed */
    QVERIFY(_detector->getMarker((hasMarker.bits()), &models) == 1);

    /* Visibility */
    QVERIFY(_banana->visible() == true);
    QVERIFY(_owl->visible() == false);

}

QTEST_APPLESS_MAIN(MarkerdetectorTest)

#include "tst_markerdetectortest.moc"
