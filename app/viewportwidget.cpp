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
#include <QPainter>
#include <QtOpenGL>
#include <QSizePolicy>
#include <QSizeF>

#ifdef Q_OS_WIN
#include <GL/glu.h>
#endif

#ifdef Q_OS_MAC
#include <glu.h>
#endif

#include <Qt3D/qglbuilder.h>
#include <Qt3D/qgraphicstranslation3d.h>
#include <Qt3D/qgraphicsrotation3d.h>

#include "defines.h"
#include "viewportwidget.h"
#include "AR/ar.h"

ViewportWidget::ViewportWidget(QWidget *parent, ImageReader *reader) :
    QGLView(parent), _reader(reader), _frame(NULL), _scale(1.00),
    _previousScale(1.00),
    _bgr(NULL)
{

}


void ViewportWidget::initializeGL(QGLPainter *painter) {
    QGLCamera *camera = new QGLCamera();
    camera->setNearPlane(5.0f);
    camera->setFarPlane(1000.0f);
    camera->setFieldOfView(34.0f);
    setCamera(camera);

    _lp.setPosition(QVector3D(-1.0, -2.0, 1.0));
    _lp.setSpotAngle(180);
    _lp.setSpotExponent(24);
    _lp.setSpotDirection(QVector3D(1.0, 4.0, 4.0));

    _lm.setAmbientSceneColor(QColor(200, 200, 200));

    painter->setLightModel(&_lm);
    painter->setMainLight(&_lp);
    painter->setClearColor(Qt::black);
}

void ViewportWidget::buildPane() {
    QGLBuilder builder;

    if (_bgr) {
        delete _bgr;
        _bgr = NULL;
    }

    builder.newSection();

    QGraphicsTranslation3D *trans = new QGraphicsTranslation3D();
    QGraphicsRotation3D *rot = new QGraphicsRotation3D();
    trans->setTranslate(QVector3D(0.0f, 0.0f, -1.0f));
    rot->setAxis(QVector3D(1, 0, 0));
    rot->setAngle(180);

    qreal size = 7.3f;
    builder.addPane(QSizeF(size, size * (qreal)SCREEN_HEIGHT /
                           (qreal)SCREEN_WIDTH));
    _bgr = builder.finalizedSceneNode();
    _bgr->addTransform(trans)   ;
    _bgr->addTransform(rot);
    _bgrImage = QImage(SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_RGB888);
    _texture = new QGLTexture2D();
    _texture->setImage(_bgrImage);
}



void ViewportWidget::drawVideoFrame(QGLPainter *painter) {
    /* Video background frame */
    if (_frame) {

        /* Following two lines just fill the fb with image. Which
         * is much nicer than the pane version. Unfortunately, doesn't
         * work in every OpenGL stack with Qt3D: Drawing a QGLSceneNode
         * erases it. Left as a placeholder.
        glPixelZoom(_pixelRatio, _pixelRatio);
        glDrawPixels(_frame->width, _frame->height, GL_RGB, GL_UNSIGNED_BYTE,
                     _frame->imageData);
        */
        if (!_bgr || !_bgrImage.bits()) {
            return;
        }
        /* Windows & Mac: QGLSceneNode::draw() erases opengl
           framebuffer, so we draw a plane with texturemapping
           instead of just filling the framebuffer with an image */

        /* FIXME: there has to be a better way to do this */
        memcpy(_bgrImage.bits(), _frame->imageData, _frame->imageSize);
        _texture->setImage(_bgrImage);
        painter->setStandardEffect(QGL::LitDecalTexture2D);
        _texture->bind();
        painter->modelViewMatrix().push();
        painter->projectionMatrix().push();
        _bgr->draw(painter);
        painter->projectionMatrix().pop();
        painter->modelViewMatrix().pop();
        _texture->release();
    }
}


void ViewportWidget::paintGL(QGLPainter *painter) {
    glEnable(GL_BLEND);
    glEnable (GL_LINE_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    /* Draw the background video */
    drawVideoFrame(painter);

    /* Draw the models on top of the video */
    glEnable(GL_DEPTH_TEST);
    QListIterator<Model3D*> i(*_models);
    while(i.hasNext()) {
        Model3D *model = i.next();
        model->draw(painter);
    }

    glDisable(GL_DEPTH_TEST);
    glDisable (GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}


/* Set background image and 3d models */
void ViewportWidget::setStatus(IplImage *rgb, IplImage *gray,
                               QList<Model3D *> *models) {
    Q_UNUSED(gray);
    _frame = rgb;
    _models = models;
    updateGL();
}


QSize ViewportWidget::calcDrawArea(int parentWidth, int parentHeight) {
    qreal w;
    qreal h;
    qreal aspect = (qreal)SCREEN_HEIGHT / (qreal)SCREEN_WIDTH;

    w = parentWidth;
    h = w * aspect;

    if (h > parentHeight) {
        h = parentHeight;
        w = h / aspect;
    }

    //qDebug() << "aspect" << aspect << "w:" << w << "h:" << h;

    return QSize((int)w, (int)h);
}

void ViewportWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    Q_ASSERT(this->parentWidget());
    this->resize(calcDrawArea(this->parentWidget()->width(),
                 this->parentWidget()->height()));
    this->setGeometry(this->parentWidget()->width() / 2 - this->width() / 2,
                      this->parentWidget()->height() / 2 - this->height() / 2,
                      this->width(), this->height());

    /* This if fb filling background draw is wanted instead of buildPane()
    _pixelRatio = (qreal)this->width() / (qreal)SCREEN_WIDTH;
    */
    buildPane();
}


void ViewportWidget::mousePressEvent ( QMouseEvent * event ) {
    event->ignore();
}

void ViewportWidget::mouseReleaseEvent ( QMouseEvent * event ) {
    Q_UNUSED(event);
}

/* Scale objects */
void ViewportWidget::wheelEvent(QWheelEvent *event) {
    int numDegrees = event->delta() / 8;
    _previousScale = _scale;
    _scale += (float)numDegrees * 0.01;
    if (_scale < 0) _scale = 0;
}


