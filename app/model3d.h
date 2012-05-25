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

#ifndef MODEL3D_H
#define MODEL3D_H

#include <QObject>
#include <QString>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <AR/ar.h>
#include <Qt3D/qglabstractscene.h>


/* ARToolkit coordinates & marker info */
typedef struct {
    double markerCoord[4][2];
    double trans[3][4];
    double markerWidth;
    double markerCenter[2];
    double vertex[2][4];
} coordinates_t;

/* Representation of a 3D model containing
   transformation matrix and corresponding marker data */
class Model3D : public QObject {
    Q_OBJECT

public:
    explicit Model3D(QObject *parent = 0);

    /* Loads 3ds object */
    virtual bool loadModel(QString &fileName);
    /* Sets the name of the object */
    virtual void setName(QString &name);
    /* Loads artoolkit pattern and id */
    virtual bool setPattern(QString &pattern);
    /* Sets the visibility */
    virtual void setVisible(bool visible);
    /* Sets object temperature */
    virtual void setTemp(qreal temp);


    /* Object scale */
    virtual void setScale(qreal scale);
    /* Updates model position & rotation */
    virtual void updateTransMat(double trans[3][4]);
    /* For testing purposes */
    virtual void setTransMat(QMatrix4x4 &mat);
    /* Converts artoolkit translation matrix to Qt3D */
    virtual void convertMat(double trans[3][4], double oglMat[16]);

    virtual QString &name();
    virtual QString &pattern();
    virtual int id();
    virtual bool visible();
    virtual qreal temp();
    virtual bool coordPtr(coordinates_t **coordinates);
    virtual QMatrix4x4& transMat();
    virtual QGLAbstractScene *scene();
    virtual qreal scale();
    virtual QGLSceneNode *mainNode();

    virtual void draw(QGLPainter *painter);

public slots:
    virtual void animate();

signals:
    void coordsUpdated();

protected:
    QMatrix4x4 _transMat;
    double _scale;
    qreal _temp;

private:
    QString _name;
    QString _pattern;
    int _id;
    bool _visible;
    coordinates_t _coordinates;
    QGLAbstractScene *_scene;
};

#endif // MODEL3D_H
