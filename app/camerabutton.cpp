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

#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QDebug>
#include <QStyleOption>
#include "camerabutton.h"

CameraButton::CameraButton(QWidget *parent) :
    MenuButton(parent), _bgImage(NULL),
    _logo(QImage("graphics/camera_logo.png"))
{
    retranslate();

}

void CameraButton::setStatus(IplImage *rgb,
                             IplImage *gray,
                             QList<Model3D*> *models) {
    Q_UNUSED(rgb);
    Q_UNUSED(models);
    Q_UNUSED(rgb);
    _bgImage = gray;
    update();
}

void CameraButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (_bgImage) {
        QPainter paint(this);

        QImage bg((uchar*)_bgImage->imageData, _bgImage->width, _bgImage->height,
                  QImage::Format_ARGB32_Premultiplied);
        QRect rect;
        /* FIXME dogvomit */
        rect.setX(10);
        rect.setY(10);
        rect.setWidth(this->width() - 20);
        rect.setHeight(this->height() - 20);
        paint.drawImage(rect, bg);
        QPen pen(Qt::black, 10, Qt::SolidLine);
        rect.setX(5);
        rect.setY(5);
        rect.setWidth(this->width() - 10);
        rect.setHeight(this->height() - 10);
        paint.setPen(pen);
        paint.drawRoundedRect(rect, 15, 15);

        paint.drawImage(QRect(this->width() / 2 - _logo.width() / 2,
                        this->height() / 2 - _logo.height() / 2,
                              _logo.width(), _logo.height()),
                         _logo);

    }
}

void CameraButton::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        retranslate();
    }
    QWidget::changeEvent(event);
}

void CameraButton::retranslate() {
    this->setToolTip(tr("Change camera"));
}
