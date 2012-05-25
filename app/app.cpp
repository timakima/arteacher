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
#include <QTranslator>
#include <QDesktopWidget>
#include <QIcon>
#include "app.h"


App::App(QObject *parent) :
    QObject(parent)
{

    /* Changes application language on runtime */
    _languageSelector = new LanguageSelector();

    QDesktopWidget *desktop = QApplication::desktop();

    /* Widget that contains OpenGL view and info view */
    _screenWidget = new ScreenWidget();
    _screenWidget->setGeometry(desktop->rect());
    _viewportWidget = new ViewportWidget(_screenWidget);
    _infoWidget = new InfoWidget(_screenWidget);

    /* Main menu */
    _mw = new MainWindow(desktop, _screenWidget, _infoWidget,
                                    desktop->width(), desktop->height());

    /* Set up controller */
    _imageReader = new ImageReader();
    _controller = new Controller(_mw, _imageReader, _languageSelector);

    /* Metadata for debugging */
    _screenWidget->setObjectName(QString("ScreenWidget"));
    _viewportWidget->setObjectName(QString("ViewportWidget"));
    _infoWidget->setObjectName(QString("InfoWidget"));
    _mw->setObjectName(QString("MainWindow"));

}

bool App::initAR() {
    bool ret;

    /* Update background image in OpenGL widget */
    connect(_controller, SIGNAL(setStatus(IplImage*, IplImage*,
                                          QList<Model3D*>*)),
            _viewportWidget, SLOT(setStatus(IplImage*, IplImage*,
                                            QList<Model3D*>*)));

    /* Create UI and connect buttons to controller */
    _mw->initUi(_controller);

    if (!_controller->initAR()) {
        qDebug() << "Error initializing AR controller";
        die();
        ret = false;
    } else {
        ret = true;
    }
    return ret;
}

void App::die() {
    _mw->close();
    delete _controller;
    delete _imageReader;
    delete _mw;
    delete _languageSelector;
}
