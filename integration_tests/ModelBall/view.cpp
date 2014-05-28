#include <QDebug>
#include <qgraphicstranslation3d.h>
#include <QTimer>
#include "modelball.h"
#include "view.h"

View::View(QWidget *parent) :
    QGLView(parent), _model(0), _updateTimer(new QTimer(this))
{
    _updateTimer->setInterval(10);
    _updateTimer->setSingleShot(true);
    connect(_updateTimer, SIGNAL(timeout()),
            this, SLOT(update()));
    _updateTimer->start();
}

void View::setModel(Model3D *model)
{
    const QVector3D position(0.5, -40, -100);
    _model = model;
    QGraphicsTranslation3D *trans = new QGraphicsTranslation3D();
    trans->setTranslate(position);
    ModelBall *modelBall = qobject_cast<ModelBall*>(_model);
    foreach(QGLSceneNode *node, modelBall->_nodes) {
        node->addTransform(trans);
    }
}

void View::initializeGL(QGLPainter *painter) {
    const float nearPlane = 1.0f;
    const float farPlane = 1000.0f;
    const float fov = 34.0f;
    QGLCamera *camera = new QGLCamera();
    camera->setNearPlane(nearPlane);
    camera->setFarPlane(farPlane);
    camera->setFieldOfView(fov);
    setCamera(camera);
    painter->setClearColor(Qt::black);
}


void View::paintGL(QGLPainter *painter)
{
    glEnable(GL_BLEND);
    glEnable (GL_LINE_SMOOTH);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

    /* Draw the models on top of the video */
    glEnable(GL_DEPTH_TEST);

    if (_model && _model->mainNode()) {
       painter->projectionMatrix().push();
       painter->modelViewMatrix().push();
       painter->modelViewMatrix() = _model->transMat();
       ModelBall *modelBall = qobject_cast<ModelBall*>(_model);
       foreach(QGLSceneNode *node, modelBall->_nodes) {
           node->draw(painter);
       }
       painter->modelViewMatrix().pop();
       painter->projectionMatrix().pop();
    }
    glDisable(GL_DEPTH_TEST);
    glDisable (GL_LINE_SMOOTH);
    glDisable(GL_BLEND);

}

void View::update()
{
    QGLView::update();
    _updateTimer->start();
}
