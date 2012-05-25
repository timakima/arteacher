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
#include <Qt3D/qglscenenode.h>
#include "model3d.h"

class Model3dTest : public QObject
{
    Q_OBJECT
    
public:
    Model3dTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void loadModel();
    void convertMat();
    void updateCoords();
    void setters();

private:
    Model3D _model;
};

Model3dTest::Model3dTest()
{
}

void Model3dTest::initTestCase() {

}

void Model3dTest::cleanupTestCase() {

}

void Model3dTest::loadModel()
{
   /* FIXME */
    QString fileName = "../models/penguin.3ds";
    _model.loadModel(fileName);

    QGLAbstractScene *scene = _model.scene();
    QVERIFY(scene != NULL);
    QGLSceneNode *node = _model.mainNode();
    QVERIFY(node != NULL);
}

void Model3dTest::convertMat() {
    /* Verify that the translation matrix is correct */
    double trans[3][4];
    double result[16];
    double expResult[16];

    trans[0][0] = 0.00;
    trans[0][1] = 0.01;
    trans[0][2] = 0.02;
    trans[0][3] = 0.03;
    trans[1][0] = 0.10;
    trans[1][1] = 0.11;
    trans[1][2] = 0.12;
    trans[1][3] = 0.13;
    trans[2][0] = 0.20;
    trans[2][1] = 0.21;
    trans[2][2] = 0.22;
    trans[2][3] = 0.23;

    expResult[0] = 0.00;
    expResult[1] = 0.10;
    expResult[2] = 0.20;
    expResult[4] = 0.01;
    expResult[5] = 0.11;
    expResult[6] = 0.21;
    expResult[8] = 0.02;
    expResult[9] = 0.12;
    expResult[10] = 0.22;
    expResult[12] = 0.03;
    expResult[13] = 0.13;
    expResult[14] = 0.23;

    _model.convertMat(trans, result);

    for (int i = 0; i < 16; i++) {
        if (i != 3 && i != 7 && i != 11 && i != 15) {
            QVERIFY(expResult[i] ==  result[i]);
        }
    }
}

void Model3dTest::updateCoords() {
    qreal transMat[16];
    qreal expTransMat[16];
    memset(transMat, 0, 16 * sizeof(qreal));
    memset(expTransMat, 0, 16 * sizeof(qreal));

    QGLAbstractScene *scene = _model.scene();
    QGLSceneNode *mainNode = scene->mainNode();
    QVERIFY(mainNode != NULL);
    coordinates_t *coords;
    QVERIFY(_model.coordPtr(&coords) == true);
    double trans[3][4];
    trans[0][0] = 0.10;
    trans[0][1] = 0.15;
    trans[0][2] = 0.20;
    trans[0][3] = 0.25;
    trans[1][0] = 0.30;
    trans[1][1] = 0.35;
    trans[1][2] = 0.40;
    trans[1][3] = 0.45;
    trans[2][0] = 0.50;
    trans[2][1] = 0.55;
    trans[2][2] = 0.60;
    trans[2][3] = 0.65;

    QVERIFY(!memcmp(transMat, expTransMat, 16 * sizeof(qreal)));
    _model.updateTransMat(trans);

    QMatrix4x4 mat = _model.transMat();
    QVERIFY(memcmp(mat.data(), expTransMat, 16 * sizeof(qreal)));

}


void Model3dTest::setters() {
    QString name("narf");
    QString pattern("../markers/marker_a.pat");
    qreal scale = 13.4;

    _model.setName(name);
    _model.setPattern(pattern);
    _model.setVisible(true);

    _model.setScale(scale);
    QCOMPARE(_model.scale(), scale);

    QCOMPARE(name, _model.name());
    QVERIFY(_model.id() != -1);
    QVERIFY(_model.visible() == true);
    _model.setVisible(false);
    QVERIFY(_model.visible() == false);

    _model.setTemp(10.0);
    QCOMPARE(_model.temp(), 10.0);

    QMatrix4x4 mat(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    _model.setTransMat(mat);
    QMatrix4x4 retMat = _model.transMat();
    QCOMPARE(mat, retMat);

}

QTEST_APPLESS_MAIN(Model3dTest)

#include "tst_model3dtest.moc"
