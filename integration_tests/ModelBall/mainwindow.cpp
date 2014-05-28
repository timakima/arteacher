#include <QHBoxLayout>
#include <QSlider>
#include "mainwindow.h"
#include "view.h"
#include "modelball.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *widget = new QWidget();
    View *view = new View();
    layout->addWidget(view);
    QSlider *slider = new QSlider();
    slider->setMaximum(MAX_TEMP);
    slider->setMinimum(MIN_TEMP);
    slider->setSliderPosition(DEFAULT_TEMP);
    layout->addWidget(slider);
    widget->setLayout(layout);
    ModelBall *ball = new ModelBall();
    ball->setVisible(true);
    ball->setTemp(DEFAULT_TEMP);
    view->setModel((Model3D*)ball);
    setCentralWidget(widget);
    showMaximized();
    connect(slider, SIGNAL(sliderMoved(int)),
            this, SLOT(onSliderMoved(int)));
    connect(this, SIGNAL(temperatureChanged(qreal)),
            ball, SLOT(setTemp(qreal)));
}

void MainWindow::onSliderMoved(int position)
{
    emit temperatureChanged((qreal)position);
}

