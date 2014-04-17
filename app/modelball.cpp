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
#include <qglpainter.h>
#include <qmath.h>
#include <QDateTime>
#include <QTimeLine>
#include "defines.h"
#include "modelball.h"


#define SPECULAR_COLOR 160, 128, 160
#define AMBIENT_COLOR 30, 0, 40
#define EMITTED_LIGHT_COLOR 5, 5, 20
#define SHININESS 99

ModelBall::ModelBall(QObject *parent) :
    Model3D(parent), _timeLine(0)
{
    _scale = 2.0;
    createScene();
    _timeLine = new QTimeLine();
    connect(_timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(animateBall(int)));
    _timeLine->start();


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
double ModelBall::randomPosition(int seed) {
    double rand;
    if (seed =! -1) {
        srand(seed);
    }
    rand = qrand();

    rand -= RAND_MAX/2;
    rand /= RAND_MAX;
    return rand;
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
                                      parent->scale() * 1.2, 0.0f));
    }
    _mainNode->addTransform(&_rot);
    _mainNode->addTransform(&_trans);

}

QGLSceneNode *ModelBall::mainNode() {
    return _mainNode;
}

void ModelBall::draw(QGLPainter *painter) {
    if (!visible()) {
        return;
    }

    painter->projectionMatrix().push();
    painter->modelViewMatrix().push();
    painter->modelViewMatrix() = transMat();
    painter->modelViewMatrix().scale(scale());
    _mainNode->draw(painter);
    painter->modelViewMatrix().pop();
    painter->projectionMatrix().pop();
}


/* Creates a timeline for animation */
QTimeLine *ModelBall::createTimeLine(int duration, int minFrame,
                                int maxFrame,
                                int updateInterval,
                                QTimeLine::CurveShape shape,
                                const char *slot) {
    _timeLine->setDuration(duration);
    _timeLine->setFrameRange(minFrame, maxFrame);
    _timeLine->setLoopCount(0);
    _timeLine->setUpdateInterval(updateInterval);
    _timeLine->setCurveShape(shape);
    return _timeLine;
}

/* Creates and starts animations for the scene and balls */
void ModelBall::startAnimation() {
    qDebug() << Q_FUNC_INFO;

    _timeLine->setDuration(10000);
    _timeLine->setFrameRange(0, 10000);
    _timeLine->setLoopCount(0);
    qDebug() << _tempScale;
    _timeLine->setUpdateInterval(_tempScale * 10);
    _timeLine->setCurveShape(QTimeLine::EaseInOutCurve);
}

/* Animates the scene and balls */
void ModelBall::animateBall(int feed) {


    /* Scale according to temperature */
    _tempScale = tempScale() * 3.0 + 0.1;

    /* Rotate scene */

    _rot.setAxis(QVector3D(1, 0, 0));
    _rot.setAngle(_xAngle);
    _rot.setAxis(QVector3D(0, 1, 0));
    _rot.setAngle(_yAngle);
    _rot.setAxis(QVector3D(0, 0, 1));
    _rot.setAngle(_zAngle);

    /* This is basicly nonsense that randomizes individual ball movement */
    qreal x;
    qreal y;
    qreal z;
    int i = 0;
    QListIterator<QGraphicsTranslation3D*> balls(_translations);
    while (balls.hasNext()) {
        x = randomPosition();
        y = randomPosition();
        z = randomPosition();

        QGraphicsTranslation3D *trans = balls.next();
        QVector3D trvec(x,
                        y,
                        z);
        trvec *= _tempScale * 10.0;
        trans->setTranslate(trvec);
        i++;
    }
}

void ModelBall::setTemp(qreal temp)
{
    if (qAbs(temp - _temp) > 5) {
        Model3D::setTemp(temp);
        startAnimation();

    }

}

bool ModelBall::visible(int markerId) {
    Q_UNUSED(markerId);
    Model3D *thermo = qobject_cast<Model3D*>(parent());
    if (thermo) {
        return thermo->visible();
    } else {
        return false;
    }

}

QMatrix4x4 ModelBall::transMat()
{
    Model3D *thermo = qobject_cast<Model3D*>(parent());
    if (thermo) {
        return thermo->transMat();
    } else {
        return QMatrix4x4();
    }
}
