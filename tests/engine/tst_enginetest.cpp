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
#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QMatrix4x4>
#include <qmath.h>
#include "tool.h"
#include "engine.h"
#include "defines.h"




class EngineTest : public QObject
{
    Q_OBJECT
    
public:
    EngineTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void distance_data();
    void distance();
    void setters();
    void tempInDistance_data();
    void tempInDistance();
    void thermoTemp_data();
    void thermoTemp();
    void addInfoField();
    void velocity_data();
    void velocity();

private:
    Engine *_engine;
};

EngineTest::EngineTest()
{
}

void EngineTest::initTestCase() {
    _engine = new Engine();
}

void EngineTest::cleanupTestCase() {
    delete _engine;
}


void EngineTest::distance_data() {
    QTest::addColumn<QVector4D>("A");
    QTest::addColumn<QVector4D>("B");
    QTest::addColumn<qreal>("result");

    QTest::newRow("1") << QVector4D(-10, -10, -10, 0)
                    << QVector4D(0, 0, 0, 0)
                    << 17.3205;

    QTest::newRow("2") << QVector4D(0, 0, 0, 0)
                    << QVector4D(10, 10, 10, 0)
                    << 17.32;

    QTest::newRow("3") << QVector4D(0, 0, 0, 0)
                    << QVector4D(0, 0, 0, 0)
                    << 0.0;

    QTest::newRow("4") << QVector4D(10, 10, 10, 0)
                    << QVector4D(10, 10, 10, 0)
                    << 0.0;

    QTest::newRow("5") << QVector4D(-10, -10, -10, 0)
                    << QVector4D(10, 10, 10, 0)
                    << 34.64;

}



void EngineTest::distance() {
    QMatrix4x4 mA;
    QMatrix4x4 mB;

    QFETCH(QVector4D, A);
    QFETCH(QVector4D, B);
    QFETCH(qreal, result);
    mA.setColumn(3, A);
    mB.setColumn(3, B);
    QVERIFY(fuzzyCompare(_engine->distance(mA, mB), result) == true);

}

void EngineTest::setters() {
    Model3D *fridge = new Model3D();
    Model3D *stove = new Model3D();
    Model3D *thermo = new Model3D();
    Model3D *balls = new Model3D();
    Model3D *display = new Model3D();

    _engine->setModels(fridge, stove, thermo, balls, display);
    QVERIFY(_engine->fridge() != NULL);
    QVERIFY(_engine->stove() != NULL);
    QVERIFY(_engine->thermo() != NULL);
    QVERIFY(_engine->balls() != NULL);
    _engine->setTemp(25.0);
    QVERIFY(fuzzyCompare(_engine->temp(), 25.0) == true);
    _engine->setTempDistance(100.0);
    QVERIFY(fuzzyCompare(_engine->tempDistance(), 100.0) == true);
}

void EngineTest::tempInDistance_data() {
    QTest::addColumn<qreal>("radius");
    QTest::addColumn<qreal>("distance");
    QTest::addColumn<qreal>("targetTemp");
    QTest::addColumn<qreal>("result");

    int i = 0;
    /* Hot */
    QTest::newRow(rowN()) << 100.0 << 101.0 << 150.0 << 0.0;
    QTest::newRow(rowN()) << 100.0 << 0.0   << 150.0 << 150.0;
    QTest::newRow(rowN()) << 100.0 << 50.0  << 150.0 << 75.0;
    QTest::newRow(rowN()) << 100.0 << 10.0  << 150.0 << 135.0;

    /* Cool */
    QTest::newRow(rowN()) << 100.0 << 101.0 << -30.0 << 0.0;
    QTest::newRow(rowN()) << 100.0 << 0.0   << -30.0 << -30.0;
    QTest::newRow(rowN()) << 100.0 << 50.0  << -30.0 << -15.0;
    QTest::newRow(rowN()) << 100.0 << 10.0  << -30.0 << -27.0;

}

void EngineTest::tempInDistance() {
    QFETCH(qreal, radius);
    QFETCH(qreal, distance);
    QFETCH(qreal, targetTemp);
    QFETCH(qreal, result);

    QVERIFY(fuzzyCompare(_engine->tempInDistance(radius, distance, 0.0,
                                                 targetTemp), result) == true);
}



void EngineTest::thermoTemp_data() {
    QTest::addColumn<QVector4D>("Fridge");
    QTest::addColumn<QVector4D>("Stove");
    QTest::addColumn<QVector4D>("Thermo");
    QTest::addColumn<qreal>("result");

    int i = 0;
    QTest::newRow(rowN()) << QVector4D(-300, 0, 0, 0)
                        << QVector4D(300, 0, 0, 0)
                        << QVector4D(0, 0, 0, 0)
                        << 25.0;

    QTest::newRow(rowN()) << QVector4D(-300, 0, 0, 0)
                        << QVector4D(300, 0, 0, 0)
                        << QVector4D(-300, 0, 0, 0)
                        << -30.0;

    QTest::newRow(rowN()) << QVector4D(-300, 0, 0, 0)
                        << QVector4D(300, 0, 0, 0)
                        << QVector4D(300, 0, 0, 0)
                        << 150.0;

}

void EngineTest::thermoTemp() {
    QMatrix4x4 mFridge;
    QMatrix4x4 mStove;
    QMatrix4x4 mThermo;

    QFETCH(QVector4D, Fridge);
    QFETCH(QVector4D, Stove);
    QFETCH(QVector4D, Thermo);
    QFETCH(qreal, result);

    mFridge.setColumn(3, Fridge);
    mStove.setColumn(3, Stove);
    mThermo.setColumn(3, Thermo);

    _engine->setTemp(25.0);
    _engine->fridge()->setTemp(-30.0);
    _engine->fridge()->setTransMat(mFridge);
    _engine->stove()->setTemp(150.0);
    _engine->stove()->setTransMat(mStove);
    _engine->thermo()->setTemp(25.0);
    _engine->thermo()->setTransMat(mThermo);

    _engine->update(false);
    QVERIFY(fuzzyCompare(_engine->thermo()->temp(), result) == true);
}

void EngineTest::addInfoField() {

}

void EngineTest::velocity_data() {
    QTest::addColumn<qreal>("temp");
    QTest::addColumn<qreal>("result");
    int i = 0;
    QTest::newRow(rowN()) << -20.0 << 325.0;
    QTest::newRow(rowN()) << 100.0 << 392.0;
    QTest::newRow(rowN()) << 20.0 << 347.33;
    QTest::newRow(rowN()) << 60.0 << 369.66;
}


void EngineTest::velocity() {
    QFETCH(qreal, temp);
    QFETCH(qreal, result);
    QVERIFY(fuzzyCompare(_engine->velocity(temp), result) == true);
}

QTEST_APPLESS_MAIN(EngineTest)

#include "tst_enginetest.moc"
