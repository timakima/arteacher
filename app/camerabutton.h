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

#ifndef CAMERABUTTON_H
#define CAMERABUTTON_H

#include <QImage>
#include "menubutton.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>


class Model3D;

/* Button class that displays video and changes camera */
class CameraButton : public MenuButton
{
    Q_OBJECT
public:
    explicit CameraButton(QWidget *parent = 0);
    void retranslate();

protected:
    virtual void paintEvent(QPaintEvent *event);
    void changeEvent(QEvent *event);

signals:

public slots:
    void setStatus(IplImage *rgb, IplImage *gray, QList<Model3D *> *models);
private:
    IplImage *_bgImage;
    QImage _logo;
    
};

#endif // CAMERABUTTON_H
