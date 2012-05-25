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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include "screenwidget.h"
#include "infowidget.h"
#include "menubutton.h"
#include "controller.h"

/* Window that contains main menu */
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0, ScreenWidget *screenWidget = 0,
                        InfoWidget *infoWidget = 0,
                        int width = 0, int height = 0);
    ~MainWindow();

    /* Scales and adds menu buttons to screen */
    void initUi(Controller *controller);
    void addLanguageButton();
    void calcButtonSize(MenuButton *button);
    void addButtonToRow(MenuButton *button);

signals:

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    /* Adds menu button with given filename as bgr image */
    void addMenuButton(QString &name, QString &background);

    /* Adds widget to info bar at the top of 3D view */
    void addInfo(INFO_WIDGET widgetType, QVariant *params,
                 QObject *sender, char *signal);

    /* Shows and hides the 3D view */
    void showSession();
    void hideSession();

    void quit();

private:
    QGridLayout *_buttonLayout;
    ScreenWidget *_screenWidget;
    InfoWidget *_infoWidget;
    int _width;
    int _height;

};

#endif // MAINWINDOW_H
