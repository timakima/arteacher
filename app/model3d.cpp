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
#include "defines.h"
#include "model3d.h"

Model3D::Model3D(QObject *parent):
    QObject(parent),
    _scale(30.0),
    _temp(0),
    _id(-1),
    _visible(false)
{
    memset((void*)&_coordinates, 0, sizeof(coordinates_t));
    _transMat.setToIdentity();
    _coordinates.markerWidth = 34.4;

}

bool Model3D::loadModel(QString &fileName) {
    _scene = QGLAbstractScene::loadScene(fileName);
    if (!_scene) {
        qDebug() << "File '" << fileName <<
                    "' not found or format not supported";
        return false;
    } else {
        return true;
    }
}

void Model3D::setName(QString &name) {
    _name = name;
}

bool Model3D::setPattern(QString &pattern) {
    _pattern = pattern;
    _id = arLoadPatt(this->_pattern.toLocal8Bit());
    if (_id == -1) {
        qDebug() << "Error loading marker pattern. Incorrect filename '"
                 << pattern << "'?";
        return false;
    } else {
        return true;
    }
}


void Model3D::setVisible(bool visible) {
    _visible = visible;
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



void Model3D::updateTransMat(double trans[3][4]) {
    qreal matrix[16];
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

    _transMat = QMatrix4x4(matrix);
    _transMat = _transMat.transposed();

}

void Model3D::setTransMat(QMatrix4x4 &mat) {
    _transMat = mat;
}


QString &Model3D::name() {
    return _name;
}

QString &Model3D::pattern() {
    return _pattern;
}

int Model3D::id() {
    return _id;
}

bool Model3D::visible() {
    return _visible;
}

qreal Model3D::temp() {
    return _temp;
}

bool Model3D::coordPtr(coordinates_t **coordinates) {
    *coordinates = &_coordinates;
    return true;
}

QMatrix4x4& Model3D::transMat() {
    return _transMat;
}

QGLAbstractScene *Model3D::scene() {
    return _scene;
}

qreal Model3D::scale() {
    return _scale;
}

QGLSceneNode *Model3D::mainNode() {
    return _scene->mainNode();
}


void Model3D::draw(QGLPainter *painter) {
    _scene->mainNode()->draw(painter);
}

void Model3D::animate() {

}


