#ifndef VIEW_H
#define VIEW_H

#include <Qt3D/qglview.h>
#include "model3d.h"

class QTimer;

class View : public QGLView
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = 0);
    void setModel(Model3D *model);

public slots:
    virtual void update();

protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);

signals:

public slots:

private:
    QTimer *_updateTimer;
    Model3D *_model;
};

#endif // VIEW_H
