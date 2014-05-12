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

#ifndef MODELDISPLAY_H
#define MODELDISPLAY_H

#include <qgraphicstranslation3d.h>
#include <qgraphicsrotation3d.h>
#include <QImage>
#include "model3d.h"

class ModelDisplay : public Model3D
{
    Q_OBJECT
public:
    explicit ModelDisplay(QObject *parent = 0);
    void buildPane();
    virtual QGLSceneNode *mainNode();
    virtual void draw(QGLPainter *painter);
    virtual bool visible(int markerId = -1);
    virtual QMatrix4x4 transMat();

signals:
    
public slots:

protected:
    void setTemp(qreal temp);

private:
    QGLSceneNode *_mainNode;
    QGraphicsRotation3D _rot;
    QGraphicsTranslation3D _trans;
    QList<QGraphicsTranslation3D*> _translations;

    QImage _displayValue;
    QGLTexture2D *_texture;
    bool _parentVisible;
    QFont _font;
    
};

#endif // MODELDISPLAY_H
