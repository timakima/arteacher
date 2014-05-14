#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include "widget.h"
#include "defines.h"
#include "boltzmanninfowidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setLayout(new QVBoxLayout());
    b = new BoltzmannInfoWidget();
    layout()->addWidget(b);
    temp = new QLineEdit("temperature");
    temp->setSelection(0, temp->text().size());
    vel = new QLabel("velocity");
    layout()->addWidget(temp);
    layout()->addWidget(vel);
    connect(temp, SIGNAL(returnPressed()),
            this, SLOT(updateTemp()));

    showMaximized();
    vel->setMaximumSize(temp->size());
}



void Widget::updateTemp()
{
    qreal t = temp->text().toInt();
    qreal k = (MAX_VELOCITY - MIN_VELOCITY) / (MAX_TEMP - MIN_TEMP);
    qreal v = MIN_VELOCITY + k * (t - MIN_TEMP);
    vel->setText(QString::number(v));
    b->setTemp(t);
    b->setVel(v);
    b->update();
}
