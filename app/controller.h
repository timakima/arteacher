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


#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QTranslator>
#include "engine.h"
#include "viewportwidget.h"
#include "markerdetector.h"
#include "languageselector.h"

class ImageReader;

/* Creates a session and controls video capture and marker detection */
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    ~Controller();
    /* Creates 3D model and maps it to ARToolkit pattern */
    bool createModel(qreal angle,
                     QVector3D rot,
                     Model3D **model,
                     QString modelFile1, QString modelFile2,
                     QString patternFile1, QString patternFile2);

    void setTranslator(QTranslator *translator);

    /* Placeholder, creates a session of markers, objects and engines */
    bool loadConfig();

    /* Begins capture and detection */
    bool initAR();

    /* Cleans up */
    void shutdownAR();

    QList<Model3D*> _models;

signals:
    /* Background video image and list of models assosiated with it */
    void setStatus(IplImage *rgb, IplImage *gray, QList<Model3D*> *models);
    void refresh(int temp, int velocity);

public slots:

    /* Transmits camera image to marker detection */
    void showImage();

    /* Opens the next camera device */
    void nextCamera();

    /* Sets application language */
    void setLanguage(QLocale::Language language);

    void toggleDebug();
    void toggleModel();

private:
    ImageReader *_reader;
    QTimer *_timer;
    MarkerDetector *_detector;
    LanguageSelector *_languageSelector;
    Engine *_engine;
};

#endif // CONTROLLER_H
