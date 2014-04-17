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

#ifndef MODELBALL_H
#define MODELBALL_H

#include <QMutex>
#include <QTimeLine>
#include <Qt3D/qglbuilder.h>
#include <Qt3D/qgraphicsrotation3d.h>
#include <Qt3D/qgraphicstranslation3d.h>
#include "model3d.h"
#include "defines.h"

/* Boltzmann gas volume indicator animation (blue animated balls moving...) */
class ModelBall : public Model3D
{
    Q_OBJECT
public:
    explicit ModelBall(QObject *parent = 0);
    void createNode(QGLBuilder &builder, QGLMaterial *material);
    void createScene();
    QTimeLine *createTimeLine(int duration, int minFrame,
                        int maxFrame,
                        int updateInterval,
                        QTimeLine::CurveShape shape,
                        const char *slot);
    virtual void startAnimation();
    virtual QGLSceneNode *mainNode();
    double randomPosition(int seed = -1);
    qreal tempScale();
    virtual void draw(QGLPainter *painter);


    virtual bool visible(int markerId = -1);
    virtual QMatrix4x4 transMat();
protected:

signals:
    
public slots:
    virtual void animateBall(int feed);
    virtual void setTemp(qreal temp);


private:
    QGLSceneNode *_mainNode;
    QGraphicsRotation3D _rot;
    QGraphicsTranslation3D _trans;
    QList<QGraphicsTranslation3D*> _translations;

    qreal _tempScale;
    int _transTable[ANIMATION_BALLS];
    int _xAngle;
    int _yAngle;
    int _zAngle;

    QTimeLine *_timeLine;
};

#endif // MODELBALL_H
