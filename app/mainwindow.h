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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLocale>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class BoltzmannInfoWidget;
class LCDWidget;
class ViewportWidget;
class Model3D;
class LanguageButton;
class ExitButton;

/* Window that contains main menu */
class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    void setupUI();
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void setStatus(IplImage *rgb, IplImage *gray, QList<Model3D *> *models);
    void refreshValues(int temp, int velocity);
    void error(QString errorMsg, QString errorTitle = "Error");

signals:
    void status(IplImage *rgb, IplImage *gray, QList<Model3D *> *models);
    void languageChanged(QLocale::Language language);
    void tempRefresh(int temp);
    void velocityRefresh(int velocity);
    void nextCamera();
    void toggleDebug();


private slots:
    void boltzmannClicked();
    void closeButtonClicked();

private:
    void placeFloatingWidgets();
    QWidget *_mainMenu;
    QWidget *_boltzmann;
    ViewportWidget *_video;
    BoltzmannInfoWidget *_coords;
    LCDWidget *_lcd;
    LanguageButton *_languageButton;
    ExitButton *_closeButton;

};

#endif // MAINWINDOW_H
