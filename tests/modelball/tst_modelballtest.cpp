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
#include <QSignalSpy>
#include "modelball.h"
#include "tool.h"
#include "defines.h"


class ModelballTest : public QObject
{
    Q_OBJECT
    
public:
    ModelballTest();
    
public slots:
    void fooSlot(int par) { }

private Q_SLOTS:
    void createScene();
    void transMat();
    void createTimeLine();
    void scaleTemp_data();
    void scaleTemp();
private:
    ModelBall _ball;
};

ModelballTest::ModelballTest()
{
}

void ModelballTest::createScene()
{
     _ball.createScene();
     QGLSceneNode *node = _ball.mainNode();
     QVERIFY(node != NULL);
}

void ModelballTest::transMat() {
    QMatrix4x4 trans = _ball.transMat();
}

void ModelballTest::createTimeLine() {
    QTimeLine *timeLine;

    timeLine = _ball.createTimeLine(100, 0,
                         1,
                         10,
                         QTimeLine::LinearCurve,
                         SLOT(rotateSceneX(int)));
    QVERIFY(timeLine != NULL);
}

void ModelballTest::scaleTemp_data() {
    QTest::addColumn<qreal>("temp");
    QTest::addColumn<qreal>("result");
    QTest::newRow("1") << MIN_TEMP << 0.0;
    QTest::newRow("2") << MAX_TEMP << 1.0;
    QTest::newRow("3") << (MIN_TEMP + MAX_TEMP) / 2.0 << 0.5;
}


void ModelballTest::scaleTemp() {
    QFETCH(qreal, temp);
    QFETCH(qreal, result);
    _ball.setTemp(temp);
    QVERIFY(fuzzyCompare(_ball.tempScale(), result) == true);
}

QTEST_APPLESS_MAIN(ModelballTest)

#include "tst_modelballtest.moc"
