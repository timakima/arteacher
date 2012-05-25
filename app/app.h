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
#ifndef APP_H
#define APP_H

#include <QObject>
#include <QApplication>
#include "screenwidget.h"
#include "controller.h"
#include "mainwindow.h"
#include "imagereader.h"
#include "viewportwidget.h"
#include "infowidget.h"
#include "languageselector.h"


/* Creates and destroys application */
class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = 0);

    /* Initializes markers and starts observing video */
    bool initAR();
public slots:

    /* Cleans up application */
    void die();
private:
    LanguageSelector *_languageSelector;
    ScreenWidget *_screenWidget;
    MainWindow *_mw;
    ViewportWidget *_viewportWidget;
    InfoWidget *_infoWidget;
    ImageReader *_imageReader;
    Controller *_controller;

};

#endif // APP_H
