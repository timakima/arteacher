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
#include <QPen>
#include <QApplication>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QHBoxLayout>
#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent) :
    QWidget(parent)
{
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setWindowFlags(Qt::FramelessWindowHint);;
    this->setLayout(new QHBoxLayout());
    //this->layout()->setContentsMargins(0, 0, 0, 0);
    //this->layout()->setMargin(0);
}

void InfoWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);



    if (parentWidget()) {
        this->setGeometry(0, 0, parentWidget()->width(),
                          parentWidget()->height() / 10);
        this->layout()->setGeometry(this->geometry());
    }

}

void InfoWidget::toggleShow() {

    if (this->isVisible()) {
        this->hide();
    } else {
        this->show();
    }
}


void InfoWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(), QBrush(QColor(0x00, 0x00, 0x00)));

}
