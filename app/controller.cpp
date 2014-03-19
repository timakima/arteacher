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
#include <QImage>
#include <QBrush>
#include <QWidget>
#include <QtOpenGL>
#include "controller.h"
#include "modelball.h"
#include "modeldisplay.h"
#include "imagereader.h"
#include "defines.h"



Controller::Controller(QObject *parent, ImageReader *reader,
                       LanguageSelector *languageSelector) :
    QObject(parent), _reader(reader), _timer(new QTimer(this)),
    _detector(new MarkerDetector()), _engine(new Engine(this)),
    _languageSelector(languageSelector)
{

}


Controller::~Controller() {
    if (_timer && _timer->isActive()) {
        _timer->stop();
    }
    delete _timer;
    delete _detector;
    delete _engine;

}


bool Controller::createModel(QString &modelFile,
                             QString &patternFile,
                             qreal angle,
                             QVector3D rot,
                             Model3D **modelptr) {
#ifndef Q_OS_LINUX
     QGraphicsTranslation3D *fix = new QGraphicsTranslation3D();
     fix->setTranslate(QVector3D(0.0f, 0.0f, 0.0f));
#endif
    bool ret;
    QMatrix4x4 trans;
    QString name;

    name.append(modelFile);
    name.append(patternFile);

    Model3D *model;
    model = new Model3D();

    if (!model->loadModel(modelFile) ||
            !model->setPattern(patternFile)) {
        goto error;
    }

    model->setName(name);

    /* Rotation */
    trans.setToIdentity();

#ifndef Q_OS_LINUX
    //model->mainNode()->addTransform(fix);
#endif
    trans.rotate(angle, rot);
    model->mainNode()->setLocalTransform(trans);

    *modelptr = model;

    ret = true;
    goto done;

    error:
    ret = false;

    done:
    return ret;
}

bool Controller::loadConfig() {

    /* FIXME placeholder code for actual configure file loading */
    /* Leaks memory, but doesn't really matter since this will be
      changed anyway */

    bool ret;
    Model3D *fridge;
    QString fridgeFile("models/blank.3ds"); //("models/fridge.3ds");
    QString fridgePat("markers/marker_a.pat");

    Model3D *stove;
    QString stoveFile("models/blank.3ds"); //("models/stove.3ds");
    QString stovePat("markers/marker_b.pat");

    Model3D *thermo;
    QString thermoFile("models/thermo.3ds");
    QString thermoPat("markers/marker_c.pat");

    Model3D *balls;
    Model3D *display;

    QVariant *tempLabelText;
    QVariant *velLabelText;
    QString boltzmannScreen;

    if (!createModel(fridgeFile,
                     fridgePat,
                    -90.0,
                    QVector3D(1, 0, 0), &fridge) ||
            !createModel(stoveFile,
                         stovePat,
                        -90.0,
                        QVector3D(1, 0, 0), &stove) ||
            !createModel(thermoFile,
                         thermoPat,
                        90.0,
                        QVector3D(0, 0, 1), &thermo)) {

        goto error;
    }

    balls = new ModelBall(thermo);
    balls->setTemp(25.0);
    stove->setTemp(100.0);
    fridge->setTemp(-20.0);
    display = new ModelDisplay(thermo);

    _models.append(fridge);
    _models.append(stove);
    _models.append(thermo);
    _models.append(balls);
    _models.append(display);

    _engine->setModels(fridge, stove, thermo, balls, display);


    tempLabelText = new QVariant("T (C)");
    velLabelText = new QVariant("v (m/s)");

    emit newInfo(WIDGET_BOLTZMANN, NULL, _engine,
                 (char*)SIGNAL(refresh(int,int)));
    emit newInfo(WIDGET_LCD, velLabelText, _engine,
                 (char*)SIGNAL(refreshVel(int)));
    emit newInfo(WIDGET_LCD, tempLabelText, _engine,
                 (char*)SIGNAL(refreshTemp(int)));


    for (int i=0; i < 1; i++) {
        QString name = "Boltzmann";
        boltzmannScreen = QString("graphics/boltzmann_screen.png");
        emit newButton(name, boltzmannScreen);
    }

    ret = true;
    goto done;

    error:
    ret = false;

    done:
    return ret;
}

void Controller::showImage() {
    IplImage *rgbImage;
    IplImage *bgraImage;
    bool detected;
    _reader->getImages(&rgbImage, &bgraImage);
    Q_ASSERT(rgbImage);
    Q_ASSERT(bgraImage);
    detected = _detector->getMarker((uchar*)bgraImage->imageData, &_models);
    _engine->update();
    emit setStatus(rgbImage, bgraImage, &_models);
}

bool Controller::initAR() {
    bool ret;
    QString camera(ARTK_DEFAULT_CAMERA);
    _detector->setParameters(SCREEN_WIDTH, SCREEN_HEIGHT, 100, camera);

    if (!loadConfig() || !_reader->initReader()) {
        goto error;
    }
    connect(_timer, SIGNAL(timeout()), this, SLOT(showImage()));
    _timer->start(30);

    ret = true;
    goto done;

    error:
    ret = false;

    done:
    return ret;
}


void Controller::shutdownAR() {
    disconnect(_timer, SIGNAL(timeout), this, SLOT(showImage()));
    /* TODO clean detector */
    if (_timer && _timer->isActive()) {
        _timer->stop();
    }
}

void Controller::nextCamera() {
    if (_timer && _reader) {
        _timer->stop();
        _reader->nextCamera();
        _timer->start(30);
    }
}

void Controller::setLanguage(QLocale::Language language) {
    if (_languageSelector) {
        _languageSelector->setLanguage(language);
    }
}
