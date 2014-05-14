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

#include <Qt3D/qglsphere.h>
#include <qglpainter.h>
#include <qmath.h>
#include <QDateTime>
#include <QTimeLine>
#include <QTimer>
#include "defines.h"
#include "modelball.h"


#define SPECULAR_COLOR 160, 128, 160
#define AMBIENT_COLOR 30, 0, 40
#define EMITTED_LIGHT_COLOR 5, 5, 20
#define SHININESS 99

ModelBall::ModelBall(QObject *parent) :
    Model3D(parent), _animationTimer(new QTimer(this))
{
    _scale = 2.0;
    createScene();

    _animationTimer->setInterval(10);
    _animationTimer->setSingleShot(true);
    connect(_animationTimer, SIGNAL(timeout()),
            this, SLOT(animate()));
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
    if (seed != -1) {
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


/* Animates the scene and balls */
void ModelBall::animate() {
    /* Scale according to temperature */
    _tempScale = tempScale() * 3.0 + 0.1;

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
        trvec *= _tempScale * 3.0 + 10; // Linear scaling according to temp
        trans->setTranslate(trvec);
        i++;
    }

    // Ball movement is not linear, but makes the point better when
    // there's a huge velocity difference between room-, fridge- and stove temps
    int interval = qSqrt(MAX_TEMP - _temp) * 5;
    _animationTimer->setInterval(interval);
    _animationTimer->start();
}

void ModelBall::setTemp(qreal temp)
{
    Model3D::setTemp(temp);
    if (!_animationTimer->isActive()) {
        _animationTimer->start();
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
