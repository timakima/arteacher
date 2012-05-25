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

#include <Qt3D/qglsphere.h>
#include <qmath.h>
#include <QDateTime>
#include <QTimeLine>
#include "defines.h"
#include "modelball.h"


#define SPECULAR_COLOR 160, 128, 160
#define AMBIENT_COLOR 30, 0, 40
#define EMITTED_LIGHT_COLOR 5, 5, 20
#define SHININESS 90

ModelBall::ModelBall(QObject *parent) :
    Model3D(parent)
{
    _scale = 4.0;
    createScene();
    startAnimation();
}


qreal ModelBall::tempScale() {
    Q_ASSERT(MAX_TEMP != MIN_TEMP);
    qreal shift = _temp - MIN_TEMP + 0.1;
    qreal range = MAX_TEMP - MIN_TEMP;
    qreal scale = shift / range;
    return scale;
}

/* Give a random position inside given scale */
double ModelBall::randomPosition() {
    return ((((double)qrand() * 2 - RAND_MAX))  / RAND_MAX);
}


/* Creates an individual randomly positioned ball */
void ModelBall::createNode(QGLBuilder &builder, QGLMaterial *material) {

    builder.newNode();
    builder << QGLSphere(1.0);
    builder.currentNode()->setPosition(QVector3D(randomPosition(),
                                                 randomPosition(),
                                                 randomPosition()));

    builder.currentNode()->setMaterial(material);
    builder.currentNode()->setEffect(QGL::LitMaterial);
    QGraphicsTranslation3D *trans = new QGraphicsTranslation3D();
    trans->setTranslate(QVector3D(1.0, 1.0, 1.0));
    builder.currentNode()->addTransform(trans);
    _translations.append(trans);

}

/* Creates the scene of balls */
void ModelBall::createScene() {
    qsrand(QDateTime::currentMSecsSinceEpoch());
    QGLMaterial *material = new QGLMaterial();
    material->setColor(Qt::blue);
    material->setSpecularColor(QColor(SPECULAR_COLOR));
    material->setAmbientColor(QColor(AMBIENT_COLOR));
    material->setEmittedLight(QColor(EMITTED_LIGHT_COLOR));
    material->setShininess(SHININESS);

    QGLBuilder builder;

    for (int i = 0; i < ANIMATION_BALLS; i++) {
        createNode(builder, material);
        _transTable[i] = qrand() % 100;
    }

    _mainNode = builder.finalizedSceneNode();
    _mainNode->setMaterial(material);
    _rot.setAngle(0.0f);
    _rot.setAxis(QVector3D(0, 0, 1));
    Model3D *parent = (Model3D*)this->parent();
    if (parent) {
        _trans.setTranslate(QVector3D(-0.3 * parent->scale() / 10,
                                      parent->scale() * 1.1, 0.0f));
    }
    _mainNode->addTransform(&_rot);
    _mainNode->addTransform(&_trans);

}

QGLSceneNode *ModelBall::mainNode() {
    return _mainNode;
}

void ModelBall::draw(QGLPainter *painter) {
    _mainNode->draw(painter);
}


/* Creates a timeline for animation */
QTimeLine *ModelBall::createTimeLine(int duration, int minFrame,
                                int maxFrame,
                                int updateInterval,
                                QTimeLine::CurveShape shape,
                                const char *slot) {
    QTimeLine *timeLine = new QTimeLine(duration, this);
    timeLine->setFrameRange(minFrame, maxFrame);
    timeLine->setLoopCount(0);
    timeLine->setUpdateInterval(updateInterval);
    timeLine->setCurveShape(shape);
    connect(timeLine, SIGNAL(frameChanged(int)),
            this, slot);
    timeLine->start();
    return timeLine;
}

/* Creates and starts animations for the scene and balls */
void ModelBall::startAnimation() {

    createTimeLine(1000, 0, 100, 10, QTimeLine::EaseInOutCurve,
                   SLOT(animateBall(int)));
    createTimeLine(5000, 0, 100, 10, QTimeLine::SineCurve,
                   SLOT(animateBall(int)));
    createTimeLine(3000, 1, 360, 10, QTimeLine::LinearCurve,
                   SLOT(rotateSceneX(int)));
    createTimeLine(3000, 1, 360, 10, QTimeLine::LinearCurve,
                   SLOT(rotateSceneY(int)));
    createTimeLine(3000, 1, 360, 10, QTimeLine::LinearCurve,
                   SLOT(rotateSceneZ(int)));

}

/* Animates the scene and balls */
void ModelBall::animateBall(int feed) {
    /* Scalle according to temperature */
    _tempScale = tempScale() * 3.0 + 0.1;

    /* Rotate scene */
    _rot.setAxis(QVector3D(1, 0, 0));
    _rot.setAngle(_xAngle);
    _rot.setAxis(QVector3D(0, 1, 0));
    _rot.setAngle(_yAngle);
    _rot.setAxis(QVector3D(0, 0, 1));
    _rot.setAngle(_zAngle);

    /* This is basicly nonsense that randomizes individual ball movement */
    int diff;
    qreal trval;
    qreal x;
    qreal y;
    qreal z;
    int i = 0;
    QListIterator<QGraphicsTranslation3D*> balls(_translations);
    while (balls.hasNext()) {
        diff = feed + _transTable[i];
        diff %= 199;
        diff -= 100;
        trval = ((qreal)diff / 100.0);
        x = trval + randomPosition() / 10.0;
        y = trval + randomPosition() / 10.0;
        z = trval + randomPosition() / 10.0;

        QGraphicsTranslation3D *trans = balls.next();
        QVector3D ovec = trans->translate().normalized();
        QVector3D trvec(x,
                        y,
                        z);
        trvec *= ovec;
        trvec *= _tempScale * 10.0;
        trans->setTranslate(trvec);
        i++;
    }
}

void ModelBall::rotateSceneX(int angle) {
    Q_UNUSED(angle);
    _xAngle += 10 * _tempScale;
}

void ModelBall::rotateSceneY(int angle) {
    Q_UNUSED(angle);
    _yAngle += 10 * _tempScale;
}

void ModelBall::rotateSceneZ(int angle) {
    Q_UNUSED(angle);
    _zAngle += 10 *_tempScale;
}
