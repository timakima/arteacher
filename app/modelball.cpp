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
    Model3D(parent), _animationTimer(new QTimer(this)), _frame(0)
{
    const float defaultScale = 2.0;
    const int interval = 10;
    _scale = defaultScale;
    createScene();
    _animationTimer->setInterval(interval);
    _animationTimer->setSingleShot(true);
    connect(_animationTimer, SIGNAL(timeout()),
            this, SLOT(animate()));
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
QGLSceneNode *ModelBall::createNode(QGLMaterial *material) {
    const float defaultSize = 1.0;
    const float positionX = -1.5;
    const float positionY = 40.0;
    const float randomPositionScale = 1000.0;
    QGLBuilder builder;
    QGLSceneNode *node;
    builder.newNode();
    builder << QGLSphere(defaultSize);
    node = builder.finalizedSceneNode();
    node->setMaterial(material);
    node->setEffect(QGL::LitMaterial);
    QGraphicsTranslation3D *trans = new QGraphicsTranslation3D();
    node->addTransform(trans);
    QGraphicsTranslation3D *transStart = new QGraphicsTranslation3D();
    QVector3D *randomStart = new QVector3D(positionX +
                                           randomPosition()/randomPositionScale,
                                           positionY +
                                           randomPosition()/randomPositionScale,
                                           randomPosition()/randomPositionScale);
    transStart->setTranslate(*randomStart);
    node->addTransform(transStart);
    return node;
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
    for (int i = 0; i < ANIMATION_BALLS; i++) {
        _nodes.append(createNode(material));
        _phase[i] = 2 * M_PI / ANIMATION_BALLS * i - M_PI;
    }
    _mainNode = _nodes.first();
}

QGLSceneNode *ModelBall::mainNode(int markerId) {
    Q_UNUSED(markerId);
    return _mainNode;
}

void ModelBall::draw(QGLPainter *painter) {
    if (parent() && !visible()) {
        return;
    }

    painter->projectionMatrix().push();
    painter->modelViewMatrix().push();
    painter->modelViewMatrix() = transMat();
    painter->modelViewMatrix().scale(scale());
    foreach(QGLSceneNode *node, _nodes) {
        node->draw(painter);
    }
    painter->modelViewMatrix().pop();
    painter->projectionMatrix().pop();
}


void ModelBall::animateBall(QGLSceneNode *node, int index, float  scale,
                            float dimension)
{
    if (node && !node->transforms().isEmpty() &&
            index >= 0 && index < ANIMATION_BALLS) {
        /* Each ball has an individual phase */
        _phase[index] += _frame;
        if (_phase[index] >  M_PI) {
            _phase[index] -= (2 * M_PI);
        }
        QGraphicsTranslation3D *trans =
                qobject_cast<QGraphicsTranslation3D*>
                (node->transforms().first());
        QVector3D vec = trans->translate();

        /* Fluff to randomize movement */
        float x = vec.x() + sin(_phase[index] * qrand()) / scale - sin(qrand()) / scale;
        float y = vec.y() + cos(_phase[index] * qrand()) / scale - sin(qrand()) / scale;
        float z = vec.z() + sin(_phase[index] * qrand()) / scale - cos(qrand()) / scale;

        /* Balls are supposed to drift from origo to temperature scaled
           dimension, then move back to origo again */
        if (qSqrt(x*x + y*y + z*z) > dimension) {
            x = 0.0;
            y = 0.0;
            z = 0.0;
        }

        vec.setX(x);
        vec.setY(y);
        vec.setZ(z);
        trans->setTranslate(vec);
    }
}

/* Animates the scene and balls */
void ModelBall::animate() {
    const float minimumScale = 1.0;
    const float minimumDimension = 8.0;
    const float maximumDimension = 10.0;
    const float minimumMovement = 0.001;
    float scale;
    float temperature;
    float dimension;

    /* Go through the whole 2 * Pi (=360 degrees) for the animation */
    _frame +=  (_temp - MIN_TEMP) / (MAX_TEMP - MIN_TEMP);
    if (_frame > M_PI) {
        _frame -= 2 * M_PI;
    }

    /* Temperature scale for individual ball movement */
    (_temp != MIN_TEMP) ? temperature = _temp + minimumMovement :
            temperature = minimumMovement;
    scale =  (MAX_TEMP - MIN_TEMP) / (temperature - MIN_TEMP) + minimumScale;

    /* Maximum dimension for the animation depends of the temperature,
       but has a reasonable minimum value */
    dimension = (_temp - MIN_TEMP) / (MAX_TEMP - MIN_TEMP)
            * maximumDimension + minimumDimension;

    int i = 0;
    foreach(QGLSceneNode *node, _nodes) {
        animateBall(node, i, scale, dimension);
        i++;
    }
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
