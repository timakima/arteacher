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

#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include <Qt3D/qglview.h>
#include <Qt3D/qglpixelbuffersurface.h>
#include <QMutex>
#include <QtOpenGL/QGLFramebufferObject>
#include "imagereader.h"
#include "model3d.h"

/* Handles OpenGL drawing with Qt3D */
/* With Linux uses glDrawPixels to draw video frame. With other OS:es
  (Win7 & OS X) drawing a Qt3D object clears the buffer so did a hack to
  draw the video as a texture mapping instead */
class ViewportWidget : public QGLView
{
    Q_OBJECT
public:
    explicit ViewportWidget(QWidget *parent = 0, ImageReader *reader = 0);


signals:
    
public slots:
    void setStatus(IplImage *rgb, IplImage *gray, QList<Model3D*> *models);
    QSize calcDrawArea(int parentWidth, int parentHeight);

protected:

    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void wheelEvent(QWheelEvent *event);
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);
    void resizeEvent(QResizeEvent *event);

private:
    void drawObject(Model3D *model, QGLPainter *painter);
    void drawVideoFrame(QGLPainter *painter);

#ifndef Q_OS_LINUX
    void buildPane();
#endif

    ImageReader *_reader;
    IplImage *_frame;

    float _scale;
    float _previousScale;

    double _pixelRatio;
    double _drawAreaWidth;
    double _drawAreaHeight;

    QList<Model3D*> *_models;
    QMutex _drawLock;

    QGLLightModel _lm;
    QGLLightParameters _lp;

#ifndef Q_OS_LINUX
    QSizeF _paneAspect;
    QGLSceneNode *_bgr;
    QGLTexture2D *_texture;
    QImage _bgrImage;
#endif
};

#endif // ViewportWidget_H
