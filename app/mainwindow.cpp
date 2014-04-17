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

#include <QApplication>
#include <QGridLayout>
#include <QDebug>
#include <QLCDNumber>
#include <QLabel>
#include "mainwindow.h"
#include "menubutton.h"
#include "languagebutton.h"
#include "camerabutton.h"
#include "staticinfowidget.h"
#include "boltzmanninfowidget.h"
#include "engine.h"
#include "defines.h"
#include "exitbutton.h"

MainWindow::MainWindow(QWidget *parent, ScreenWidget *screenWidget,
                       InfoWidget *infoWidget,
                       int width, int height) :
    QWidget(parent), _buttonLayout(NULL), _screenWidget(screenWidget),
    _infoWidget(infoWidget),
    _width(width),
    _height(height) {
    connect(_screenWidget, SIGNAL(hideSession()), this, SLOT(hideSession()));

    connect(_screenWidget, SIGNAL(clicked()), _infoWidget, SLOT(toggleShow()));
    this->showFullScreen();
}

MainWindow::~MainWindow() {
    delete _buttonLayout;
}

void MainWindow::initUi(Controller *controller) {
    setMinimumWidth(_width);
    setMinimumHeight(_height);
    this->showFullScreen();
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("Main layout");
    _buttonLayout = new QGridLayout(this);
    _buttonLayout->setObjectName("ButtonLayout");
    this->setLayout(mainLayout);

    QFont font;
    font.setPointSize(60);
    QLabel *title = new QLabel(this);
    title->setText(APP_NAME);
    title->setFont(font);
    mainLayout->addWidget(title);
    mainLayout->setAlignment(title, Qt::AlignHCenter);
    mainLayout->addLayout(_buttonLayout);
    mainLayout->addSpacing(height() / 3);

    LanguageButton *langButton = new LanguageButton(this);
    langButton->show();
    CameraButton *cameraButton = new CameraButton(this);
    ExitButton *exitButton = new ExitButton(this);
    exitButton->show();

    ExitButton *hideSessionButton = new ExitButton(_screenWidget);

    calcButtonSize(cameraButton);
    addButtonToRow(cameraButton);

    connect(controller, SIGNAL(newButton(QString&,QString&)),
            this, SLOT(addMenuButton(QString&,QString&)));

    connect(cameraButton, SIGNAL(clicked()), controller, SLOT(nextCamera()));

    connect(controller, SIGNAL(setStatus(IplImage*, IplImage*,
                                         QList<Model3D*>*)),
            cameraButton, SLOT(setStatus(IplImage*, IplImage*,
                                         QList<Model3D*>*)));

    connect(controller, SIGNAL(newInfo(INFO_WIDGET,QVariant*,QObject*,char*)),
            this, SLOT(addInfo(INFO_WIDGET,QVariant*,QObject*,char*)));

    connect(exitButton, SIGNAL(clicked()), this, SLOT(quit()));
    connect(hideSessionButton, SIGNAL(clicked()), this,
            SLOT(hideSession()));

    connect(langButton, SIGNAL(languageChanged(QLocale::Language)),
            controller, SLOT(setLanguage(QLocale::Language)));

}



void MainWindow::calcButtonSize(MenuButton *button) {
    int width = this->width();
    int buttonWidth = width / UI_BUTTONS_IN_ROW - 30;
    int buttonHeight = ((double)buttonWidth / 4.0)  * 3.0;
    button->setMaximumSize(buttonWidth, buttonHeight);
}

void MainWindow::addButtonToRow(MenuButton *button) {
    int buttonCount;
    buttonCount = _buttonLayout->count();
    int row = buttonCount % UI_BUTTONS_IN_ROW;
    int col = buttonCount / UI_BUTTONS_IN_ROW;
    _buttonLayout->addWidget(button, col, row);
}


void MainWindow::addInfo(INFO_WIDGET widgetType, QVariant *params,
                         QObject *sender, char *signal) {
    Q_ASSERT(_infoWidget);
    QWidget *infoField;
    QLCDNumber *lcd;
    BoltzmannInfoWidget *boltzmann;
    QImage *image;
    QLayout *subLayout;
    QLabel *label;
    QFont font;

    switch (widgetType) {
        case WIDGET_LCD:
        lcd = new QLCDNumber();
        lcd->setSegmentStyle(QLCDNumber::Flat);
        lcd->setFrameStyle(QFrame::NoFrame);
        font = QApplication::font();
        label = new QLabel(params->toString());
        label->setAlignment(Qt::AlignVCenter);
        font.setPointSize(20);
        label->setFont(font);
        connect((Engine*)sender, signal, lcd, SLOT(display(int)));
        subLayout = new QHBoxLayout();
        subLayout->addWidget(lcd);
        subLayout->addWidget(label);
        subLayout->setMargin(0);
        infoField = new QWidget();
        infoField->setLayout(subLayout);
        break;
        case WIDGET_IMAGE:
        image = new QImage(params->toString());
        infoField = new StaticInfoWidget(0, image);
        break;
        case WIDGET_BOLTZMANN:
        boltzmann = new BoltzmannInfoWidget();
        connect((Engine*)sender, signal, boltzmann, SLOT(refresh(int, int)));
        infoField = (QWidget*)boltzmann;
        break;
    default:
        qDebug() << "Unknown widget type. Ignoring.";
        goto done;
    }

    _infoWidget->layout()->addWidget(infoField);

    done:
    return;
}

void MainWindow::showSession() {
    Q_ASSERT(_screenWidget);
    _screenWidget->showFullScreen();
    this->hide();
}

void MainWindow::hideSession() {
    Q_ASSERT(_screenWidget);
    this->showFullScreen();
    _screenWidget->hide();
}


void MainWindow::addMenuButton(QString &name, QString &background) {
    MenuButton *menuButton = new MenuButton(this);
    menuButton->setText(name);
    QString bgr("border-image: url(");
    bgr.append(background);
    bgr.append(");");
    menuButton->setStyleSheet(bgr);

    calcButtonSize(menuButton);
    connect(menuButton, SIGNAL(clicked()),
            this, SLOT(showSession()));
    addButtonToRow(menuButton);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << __FUNCTION__;
    int key;
    key = event->key();
    if (key == Qt::Key_Escape) {
        this->quit();
    }
}


void MainWindow::quit() {
    qDebug() << __PRETTY_FUNCTION__;
    QApplication::quit();
}
