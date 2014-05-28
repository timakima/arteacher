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

#ifndef MODEL3D_H
#define MODEL3D_H

#include <QObject>
#include <QString>
#include <AR/ar.h>
#include <Qt3D/qglabstractscene.h>


#if QT_TESTLIB_LIB
#define private public
#endif

/* ARToolkit coordinates & marker info */
typedef struct {
    double markerCoord[4][2];
    double trans[3][4];
    double markerWidth;
    double markerCenter[2];
    double vertex[2][4];
} coordinates_t;

class QGLMaterial;

/* Representation of a 3D model containing
   transformation matrix and corresponding marker data */
class Model3D : public QObject {
    Q_OBJECT

public:
    explicit Model3D(QObject *parent = 0);

    /* Loads 3ds object */
    virtual bool addModel(QString fileName, QString pattern);
    /* Sets the name of the object */
    virtual void setName(QString &name);

    virtual void setLocalTransform(QMatrix4x4 trans);

    /* Sets the visibility */
    virtual void setVisible(bool visible, int markerId = -1);
    /* Sets object temperature */
    virtual void setTemp(qreal temp);


    /* Object scale */
    virtual void setScale(qreal scale);
    /* Updates model position & rotation */
    virtual void updateTransMat(double trans[3][4], int markerId = -1);
    /* For testing purposes */
    virtual void setTransMat(QMatrix4x4 mat);
    /* Converts artoolkit translation matrix to Qt3D */
    virtual void convertMat(double trans[3][4], double oglMat[16]);

    virtual QString name();
    virtual QList<int> markerIds();

    virtual bool visible(int markerId = -1);

    virtual qreal temp();
    virtual coordinates_t *coordPtr(int markerId = -1);

    virtual QMatrix4x4 transMat();
    virtual qreal scale();
    virtual QGLSceneNode *mainNode(int markerId = -1);
    virtual void draw(QGLPainter *painter);

    virtual QMatrix4x4 calcModelPosition();
    void drawDebug(QGLPainter *painter);
    bool setMarkerPositionToModel(int markerId, float x, float y, float z);
    bool markerPositionToModel(int markerId, float &x, float &y, float &z);
    void setModelVisible(bool on);
    bool modelVisible();
    void setDebug(bool on);
    bool debug();
public slots:
    virtual void animate();

signals:
    void coordsUpdated();

protected:
    /* Object center */
    QMatrix4x4 _transMat;

    /* Translations for markers */
    QHash<int, QMatrix4x4> _markerTransMats;

    double _scale;
    qreal _temp;

private:

    bool loadModelFile(QString fileName, QGLAbstractScene *&scene);
    bool setPatternFile(QString pattern, int &id);
    void drawObject(QGLPainter *painter, int markerId = -1);

    QString _name;

    QList<int> _markerIds;
    QHash<int, bool> _visible;
    QHash<int, coordinates_t*> _coords;
    QHash<int, QGLAbstractScene*> _scenes;
    QHash<int, QVector3D> _markerToModelFix;

    QGLSceneNode *_mainNode;
    QGLMaterial *_debugMaterial;
    QGLSceneNode *_debugNode;

    QGLMaterial *_debugMarkerMaterial;
    QGLSceneNode *_debugMarkerNode;


    bool _debug;
    bool _showModel;
};

#endif // MODEL3D_H
