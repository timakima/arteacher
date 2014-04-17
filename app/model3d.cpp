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
#include <QMatrix4x4>
#include <qgraphicstranslation3d.h>
#include <qglpainter.h>
#include <qglbuilder.h>
#include <qglsphere.h>
#include <qglmaterial.h>
#include "defines.h"
#include "model3d.h"

Model3D::Model3D(QObject *parent):
    QObject(parent),
    _scale(30.0),
    _temp(0),
    _mainNode(0), _debug(false), _showModel(false)
{
    _transMat.setToIdentity();
    QGLBuilder builder;
    _debugMaterial = new QGLMaterial();
    _debugMaterial->setColor(Qt::red);

    builder.newNode();
    builder << QGLSphere(5.0);
    builder.currentNode()->setMaterial(_debugMaterial);
    builder.currentNode()->setEffect(QGL::LitMaterial);
    _debugNode = builder.finalizedSceneNode();

    _debugMarkerMaterial = new QGLMaterial();
    _debugMarkerMaterial->setColor(Qt::green);

    QGLBuilder builder2;
    builder2.newNode();
    builder2 << QGLSphere(5.0);
    builder2.currentNode()->setMaterial(_debugMarkerMaterial);
    builder2.currentNode()->setEffect(QGL::LitMaterial);
    _debugMarkerNode = builder2.finalizedSceneNode();
}

void Model3D::setDebug(bool on) {
    _debug = on;
}

bool Model3D::debug()
{
    return _debug;
}

bool Model3D::addModel(QString fileName, QString pattern)
{
    bool ret = false;
    int id = -1;
    QGLAbstractScene *scene;
    ret = setPatternFile(pattern, id);
    if (ret) {
        ret &= loadModelFile(fileName, scene);
    }

    if (ret && id > -1) {
        _markerIds.append(id);
        _scenes[id] = scene;
        _coords[id] = new coordinates_t;
        _coords[id]->markerWidth = 25.0;
        _visible[id] = false;
        _markerToModelFix[id] = QVector3D(0, 0, 0);
    }

    return ret;
}


bool Model3D::loadModelFile(QString fileName, QGLAbstractScene *&scene) {
    scene = QGLAbstractScene::loadScene(fileName);
    if (!scene) {
        qDebug() << "File '" << fileName <<
                    "' not found or format not supported";
        return false;
    } else {
        return true;
    }
}


bool Model3D::setPatternFile(QString pattern, int &id) {
    id = arLoadPatt(pattern.toLocal8Bit());
    if (id == -1) {
        qDebug() << "Error loading marker pattern. Incorrect filename '"
                 << pattern << "'?";
        return false;
    } else {
        return true;
    }
}

void Model3D::setName(QString &name) {
    _name = name;
}

void Model3D::setLocalTransform(QMatrix4x4 trans)
{
    foreach(QGLAbstractScene *scene, _scenes.values()) {
        scene->mainNode()->setLocalTransform(trans);
    }
}

void Model3D::setVisible(bool visible, int markerId) {

    if (markerId == -1 && _markerIds.size() > 0) {
        markerId = _markerIds.at(0);
    }

    if (markerId >= 0 && _visible.contains(markerId)) {
        _visible[markerId] = visible;
    }
}

void Model3D::setTemp(qreal temp) {
    //qDebug() << this->objectName() << temp;
    _temp = temp;
}

void Model3D::setScale(qreal scale) {
    _scale = scale;
}

/* Convert artoolkit translation matrix to opengl matrix */
void Model3D::convertMat(double trans[3][4], double oglMat[16]) {
    memset(oglMat, 0, 16);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            oglMat[j * 4 + i] = trans[i][j];
        }
    }
}



void Model3D::updateTransMat(double trans[3][4], int markerId) {

    qreal matrix[16];
    QMatrix4x4 transMat;
    convertMat(trans, matrix);
    matrix[3] = 0.0;
    matrix[7] = 0.0;
    matrix[11] = 0.0;
    matrix[15] = 1.0;

    matrix[1] *= -1.0;
    matrix[2] *= -1.0;
    matrix[5] *= -1.0;
    matrix[6] *= -1.0;
    matrix[8] *= -1.0;
    matrix[13] *= -1.0;
    matrix[14] *= -1.0;

    transMat = QMatrix4x4(matrix).transposed();

    if (markerId == -1) {
        _transMat = transMat;
    } else {
        _markerTransMats[markerId] = transMat;
    }

    emit coordsUpdated();
}

void Model3D::setTransMat(QMatrix4x4 mat) {
    _transMat = mat;
}


QString Model3D::name() {
    return _name;
}

QList<int> Model3D::markerIds() {
    return _markerIds;
}


bool Model3D::visible(int markerId) {
    bool ret = false;
    if (markerId == -1 && _markerIds.size() > 0) {
        markerId = _markerIds.at(0);
    }

    if (markerId > -1 && _visible.contains(markerId)) {
        ret = _visible[markerId];
    } else {
        ret = false;
    }

    return ret;
}


qreal Model3D::temp() {
    return _temp;
}

coordinates_t *Model3D::coordPtr(int markerId)
{
    if (markerId == -1 && _markerIds.size() > 0
            &&_coords.contains(_markerIds.at(0))) {
        return _coords[_markerIds.at(0)];
    } else if (_coords.contains(markerId)) {
        return _coords[markerId];
    }

    /* not found */
    return 0;
}

QMatrix4x4 Model3D::calcModelPosition()
{
    /* We support two markers by default */
    QMatrix4x4 mat;
    if (_markerIds.size() < 2) {
        mat = QMatrix4x4();
    } else

    if (_visible[_markerIds.at(0)]) {
        mat = _markerTransMats[_markerIds.at(0)];
        mat.translate(_markerToModelFix[_markerIds.at(0)]);
    } else if (_visible[_markerIds.at(1)]) {
        mat = _markerTransMats[_markerIds.at(1)];
        mat.translate(_markerToModelFix[_markerIds.at(1)]);
    }
    return mat;
}

QMatrix4x4 Model3D::transMat() {

    if (_markerIds.size() <= 0) {
        /* Object gets coordinates from outside, no marker */
        return _transMat;
    } else if (_markerIds.size() == 1) {
        /* Exactly one marker, so position of the model is the same */
        return _markerTransMats[_markerIds.at(0)];
    } else {
        /* Multiple markers, have to figure out the position of the object */
        return calcModelPosition();
    }
}

qreal Model3D::scale() {
    return _scale;
}

QGLSceneNode *Model3D::mainNode(int markerId) {
    if (markerId == -1 && _markerIds.size() > 0) {
        markerId = _markerIds.at(0);
    }

    if (_scenes.contains(markerId)) {
        return _scenes[markerId]->mainNode();
    } else {
        return 0;
    }
}


void Model3D::drawObject(QGLPainter *painter, int markerId) {
    QMatrix4x4 trans;

    if (markerId == -1 && _markerIds.size() > 0) {
        markerId = _markerIds.at(0);
    }

    if (_markerTransMats.contains(markerId)) {
        trans = _markerTransMats[markerId];
    } else {
        /* No coordinates to draw */
        trans = transMat();
        return;
    }

    painter->projectionMatrix().push();
    painter->modelViewMatrix().push();
    painter->modelViewMatrix() = trans;
    painter->modelViewMatrix().scale(scale());
    if (_scenes.contains(markerId)) {
        //qDebug() << Q_FUNC_INFO << markerId << trans;
        _scenes[markerId]->mainNode()->draw(painter);
    }
    painter->modelViewMatrix().pop();
    painter->projectionMatrix().pop();
}


void Model3D::drawDebug(QGLPainter *painter) {
    painter->projectionMatrix().push();
    painter->modelViewMatrix().push();
    painter->modelViewMatrix() = transMat();
    _debugNode->draw(painter);
    painter->modelViewMatrix().pop();
    painter->projectionMatrix().pop();

    foreach(int markerId, _markerIds) {
        if (_visible.contains(markerId) && _visible[markerId]) {
            painter->projectionMatrix().push();
            painter->modelViewMatrix().push();
            painter->modelViewMatrix() = _markerTransMats[markerId];
            _debugMarkerNode->draw(painter);
            painter->modelViewMatrix().pop();
            painter->projectionMatrix().pop();
        }
    }

}

bool Model3D::setMarkerPositionToModel(int markerId, float x, float y, float z)
{
    if (_markerToModelFix.contains(markerId)) {
        _markerToModelFix[markerId] = QVector3D(x, y, z);
        return true;
    } else {
        return false;
    }
}

bool Model3D::markerPositionToModel(int markerId, float &x, float &y, float &z)
{
    if (_markerToModelFix.contains(markerId)) {
        x = _markerToModelFix[markerId].x();
        y = _markerToModelFix[markerId].y();
        z = _markerToModelFix[markerId].z();
        return true;
    } else {
        return false;
    }
}

void Model3D::draw(QGLPainter *painter) {
    if (_debug && !name().contains("thermo")) {
        drawDebug(painter);
    }
    foreach(int markerId, _markerIds) {
        if (_visible.contains(markerId) && _visible[markerId] && _showModel) {
            drawObject(painter, markerId);
        }
    }
}

void Model3D::animate() {

}

void Model3D::setModelVisible(bool on)  {
    _showModel = on;
}


bool Model3D::modelVisible()
{
    return _showModel;
}
