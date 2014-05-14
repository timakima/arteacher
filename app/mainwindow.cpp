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

#include <QHBoxLayout>
#include <QDebug>
#include <QStackedLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>
#include "mainwindow.h"
#include "menubutton.h"
#include "languagebutton.h"
#include "camerabutton.h"
#include "boltzmanninfowidget.h"
#include "defines.h"
#include "exitbutton.h"
#include "viewportwidget.h"
#include "lcdwidget.h"

const int MAINMENU_INDEX = 0;
const int BOLTZMANN_INDEX = 1;

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    _mainMenu(new QWidget(this)),
    _boltzmann(new QWidget(this)),
    _video(new ViewportWidget(this)),
    _coords(new BoltzmannInfoWidget(this)),
    _lcd(new LCDWidget(this)),
    _languageButton(new LanguageButton(this)),
    _closeButton(new ExitButton(this))
{
    _mainMenu->show();
    _boltzmann->hide();

    connect(this, SIGNAL(status(IplImage*,IplImage*,QList<Model3D*>*)),
            _video, SLOT(setStatus(IplImage*,IplImage*,QList<Model3D*>*)));

    setupUI();
}

void MainWindow::setupUI() {
    const QString boltzmannButtonBgr = "graphics/boltzmann_screen.png";
    const float buttonAspectRatio = 3.0/4.0;
    const int spacerPortion = 5;
    QStackedLayout *mainLayout = new QStackedLayout();
    mainLayout->insertWidget(MAINMENU_INDEX, _mainMenu);
    mainLayout->insertWidget(BOLTZMANN_INDEX, _boltzmann);
    setLayout(mainLayout);

    QHBoxLayout *selectionLayout = new QHBoxLayout();
    _mainMenu->setLayout(selectionLayout);

    QVBoxLayout *boltzmannLayout = new QVBoxLayout();
    boltzmannLayout->addWidget(_video);
    _boltzmann->setLayout(boltzmannLayout);

    /* Camera */
    CameraButton *cameraButton = new CameraButton();
    cameraButton->setMinimumHeight(cameraButton->width() * buttonAspectRatio);
    connect(this,
            SIGNAL(status(IplImage*,IplImage*,QList<Model3D*>*)),
            cameraButton,
            SLOT(setStatus(IplImage*,IplImage*,QList<Model3D*>*)));
    connect(cameraButton,
            SIGNAL(clicked()),
            this,
            SIGNAL(nextCamera()));

    /* Boltzman experiment */
    MenuButton *boltzmannButton = new MenuButton();
    boltzmannButton->setImage(boltzmannButtonBgr);
    boltzmannButton->setMinimumHeight(boltzmannButton->width() *
                                      buttonAspectRatio);
    connect(boltzmannButton, SIGNAL(clicked()), this, SLOT(boltzmannClicked()));
    _coords->setVisible(false);
    _lcd->setVisible(false);

    /* Selection layout */
    QSpacerItem *spacer = new QSpacerItem(width()/spacerPortion,
                                          height()/spacerPortion);
    selectionLayout->addSpacerItem(spacer);
    selectionLayout->addWidget(cameraButton);
    selectionLayout->addWidget(boltzmannButton);
    selectionLayout->addSpacerItem(spacer);

    /* Control layout */
    connect(_languageButton, SIGNAL(languageChanged(QLocale::Language)),
            this, SIGNAL(languageChanged(QLocale::Language)));
    _languageButton->show();
    connect(_closeButton, SIGNAL(clicked()),
             this, SLOT(closeButtonClicked()));
    _closeButton->show();
    placeFloatingWidgets();

    _languageButton->raise();
    _closeButton->raise();
}


void MainWindow::setStatus(IplImage *rgb,
                             IplImage *gray,
                             QList<Model3D*> *models) {

    emit status(rgb, gray, models);
}


void MainWindow::refreshValues(int temp, int velocity) {
    _lcd->updateLCD(temp, velocity);
    _coords->refresh(temp, velocity);
}

void MainWindow::error(QString errorMsg, QString errorTitle)
{

    QMessageBox box(QMessageBox::Critical,
                    errorTitle,
                    errorMsg,
                    QMessageBox::Ok);
    box.exec();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        closeButtonClicked();
        break;
    case Qt::Key_D:
        emit toggleDebug();
        break;
    default:
        break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    QStackedLayout *mainLayout = qobject_cast<QStackedLayout*>(layout());
    if (mainLayout->currentIndex() == BOLTZMANN_INDEX) {
        _video->lower();
        _coords->raise();
        _lcd->raise();
        _coords->setVisible(!_coords->isVisible());
        _lcd->setVisible(!_lcd->isVisible());
    }
    _languageButton->raise();
    _closeButton->raise();

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    placeFloatingWidgets();
    QWidget::resizeEvent(event);
}

void MainWindow::boltzmannClicked()
{
    QStackedLayout *mainLayout = qobject_cast<QStackedLayout*>(layout());
    if (mainLayout->currentIndex() == MAINMENU_INDEX) {
        mainLayout->setCurrentIndex(BOLTZMANN_INDEX);
    } else {
        /* Fall back to main menu in every other case */
        mainLayout->setCurrentIndex(MAINMENU_INDEX);
    }
    _languageButton->raise();
    _closeButton->raise();

}

void MainWindow::closeButtonClicked()
{
    QStackedLayout *mainLayout = qobject_cast<QStackedLayout*>(layout());
    if (mainLayout->currentIndex() == MAINMENU_INDEX) {
        QApplication::quit();
    } else {
        /* Fall back to main menu in every other case */
        _coords->hide();
                _lcd->hide();
        mainLayout->setCurrentIndex(MAINMENU_INDEX);
    }
    _languageButton->raise();
    _closeButton->raise();

}

void MainWindow::placeFloatingWidgets()
{
    const int buttonWidth = 50; // px, png file
    const int buttonHeight = 50; // px, png file
    const int spacer = 5; /* px */
    const float coordsXPortion = 0.4;
    const int coordsYPortion = 5;

    _languageButton->setGeometry(width() - buttonWidth * 2 - spacer, 0,
                                 buttonWidth, buttonHeight);
    _closeButton->setGeometry(width() - buttonWidth - spacer, 0,
                             buttonWidth, buttonHeight);
    _coords->setGeometry(0, 0, width() * coordsXPortion,
                         height() / coordsYPortion);
    _lcd->setGeometry(_coords->x() + _coords->width(), 0,
                      width() - _coords->width() - 2 * buttonWidth,
                      height() / 10);
}

