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

#include <QFontMetrics>
#include <QApplication>
#include <QFont>
#include <QPainter>
#include <qglbuilder.h>
#include <qglpainter.h>
#include "modeldisplay.h"


ModelDisplay::ModelDisplay(QObject *parent) :
    Model3D(parent), _displayValue(QImage(200, 100, QImage::Format_ARGB32))
{
    buildPane();
}

void ModelDisplay::buildPane() {

     _displayValue.fill(0);
    QGLBuilder builder;

    QGraphicsTranslation3D *trans = new QGraphicsTranslation3D();
    trans->setTranslate(QVector3D(-0.125f, 1.5f, -0.12f));

    builder.newSection();
    builder.addPane(QSizeF(0.9f, 0.75f));
    _mainNode = builder.finalizedSceneNode();
    _mainNode->addTransform(trans);

    _texture = new QGLTexture2D();
    _texture->setImage(_displayValue);
}

QGLSceneNode *ModelDisplay::mainNode() {
    return _mainNode;
}

void ModelDisplay::setTemp(qreal temp) {

    QFont font = QApplication::font();
    font.setPointSize(58);
    QString str(QString::number((int)temp));
    QFontMetrics metrics(font);

    QRect rect = metrics.boundingRect(str);
    rect.adjust(0, 0, 1, 1);

    _displayValue.fill(0);
    QPainter p(&_displayValue);
    p.setFont(font);
    p.setPen(Qt::red);
    p.drawText(25, 80, str);
    p.end();
}


void ModelDisplay::draw(QGLPainter *painter) {
    painter->setStandardEffect(QGL::LitDecalTexture2D);
    _texture->setImage(_displayValue);
    _texture->bind();
    _mainNode->draw(painter);
    _texture->release();
}
