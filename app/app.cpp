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

#include <QDebug>
#include <QApplication>
#include "app.h"
#include "mainwindow.h"
#include "controller.h"

App::App(QObject *parent) :
    QObject(parent)
{
    Controller *controller = new Controller(this);
    MainWindow *mw = new MainWindow(0);
    mw->hide();

    if (!controller->initAR()) {
        mw->error(tr("Unable to find any usable cameras. "
                  "Check if they are connected"), tr("Camera error"));
        QApplication::quit();
    }

    mw->showFullScreen();
    connect(controller, SIGNAL(setStatus(IplImage*,IplImage*,QList<Model3D*>*)),
            mw, SLOT(setStatus(IplImage*,IplImage*,QList<Model3D*>*)));
    connect(controller, SIGNAL(refresh(int,int)),
            mw, SLOT(refreshValues(int,int)));

    connect(mw, SIGNAL(languageChanged(QLocale::Language)),
            controller, SLOT(setLanguage(QLocale::Language)));
    connect(mw, SIGNAL(nextCamera()),
            controller, SLOT(nextCamera()));
    connect(mw, SIGNAL(toggleDebug()),
            controller, SLOT(toggleDebug()));

}
