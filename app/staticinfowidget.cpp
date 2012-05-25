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

#include <QPainter>
#include <QDebug>
#include <QPaintEvent>
#include "staticinfowidget.h"

StaticInfoWidget::StaticInfoWidget(QWidget *parent, QImage *image) :
    QWidget(parent), _image(image)
{
    Q_ASSERT(image);
}


void StaticInfoWidget::paintEvent(QPaintEvent *event) {

    QPainter paint(this);
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    paint.drawRect(this->rect());
    if (_image) {
        paint.drawImage(event->rect(), *_image);
    }
}

QSize StaticInfoWidget::sizeHint() {
    return QSize(_image->width(), _image->height());
}
