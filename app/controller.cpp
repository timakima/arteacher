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
#include <QImage>
#include <QBrush>
#include <QWidget>
#include <QtOpenGL>
#include "controller.h"
#include "modelball.h"
#include "modeldisplay.h"
#include "imagereader.h"
#include "defines.h"



Controller::Controller(QObject *parent) :
    QObject(parent),
    _reader(new ImageReader(this)),
    _timer(new QTimer(this)),
    _detector(new MarkerDetector()),
    _languageSelector(new LanguageSelector(this)),
    _engine(new Engine(this))

{
    connect(_engine, SIGNAL(refresh(int,int)), this, SIGNAL(refresh(int,int)));
}


Controller::~Controller() {
    if (_timer && _timer->isActive()) {
        _timer->stop();
    }

}


bool Controller::createModel(qreal angle,
                             QVector3D rot,
                             Model3D **modelptr,
                             QString modelFile1, QString modelFile2,
                             QString patternFile1, QString patternFile2) {
    bool ret;
    QMatrix4x4 trans;
    QString name;

    name.append(modelFile1);
    name.append(patternFile1);

    Model3D *model;
    model = new Model3D();

    ret = model->addModel(modelFile1, patternFile1);
    if (ret && modelFile2.size() > 0 && patternFile2.size() > 0) {
        ret &= model->addModel(modelFile2, patternFile2);
    }
    if (!ret) {
        goto error;
    }

    model->setName(name);

    /* Rotation */
    trans.setToIdentity();

    trans.rotate(angle, rot);
    model->setLocalTransform(trans);

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
    QMatrix4x4 trans;

    bool ret;
    Model3D *fridge;
    const QString fridgeFile("models/blank.3ds"); //("models/fridge.3ds");
    const QString fridgePat1("markers/marker_a.pat");
    const QString fridgePat2("markers/marker_b.pat");

    Model3D *stove;
    const QString stoveFile("models/blank.3ds"); //("models/stove.3ds");
    const QString stovePat1("markers/marker_c.pat");
    const QString stovePat2("markers/marker_d.pat");

    Model3D *thermo;
    const QString thermoFile("models/thermo.3ds");
    const QString thermoPat("markers/marker_e.pat");

    Model3D *balls;
    Model3D *display;

    if (!createModel(0.0,
                    QVector3D(0, 0, 0), &fridge, fridgeFile, fridgeFile, fridgePat1, fridgePat2) ||
            !createModel(
                        0.0,
                         QVector3D(0, 0, 0), &stove, stoveFile, stoveFile, stovePat1, stovePat2) ||
            !createModel(
                        90.0,
                         QVector3D(0, 0, 1), &thermo, thermoFile, "", thermoPat, "")) {

        goto error;
    }

    balls = new ModelBall(thermo);
    balls->setTemp(25.0);
    stove->setTemp(MAX_TEMP);
    fridge->setTemp(MIN_TEMP);
    thermo->setModelVisible(true);

    if (fridge->markerIds().size() >= 2) {
        fridge->setMarkerPositionToModel(fridge->markerIds().at(0), -20, 55,-40);
        fridge->setMarkerPositionToModel(fridge->markerIds().at(1), -10, 0, 100);
    }

    if (stove->markerIds().size() >= 2) {
        stove->setMarkerPositionToModel(stove->markerIds().at(0), 60, 0, -20);
        stove->setMarkerPositionToModel(stove->markerIds().at(1), -60, 0, -20);
    }

    trans.setToIdentity();
    trans.translate(0, -1.0, 0);
    trans.rotate(90, QVector3D(0,0,1));
    thermo->setLocalTransform(trans);

    display = new ModelDisplay(thermo);

    _models.append(fridge);
    _models.append(stove);
    _models.append(thermo);
    _models.append(balls);
    _models.append(display);

    _engine->setModels(fridge, stove, thermo, balls, display);

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
    _reader->getImages(&rgbImage, &bgraImage);
    Q_ASSERT(rgbImage);
    Q_ASSERT(bgraImage);
    bool detected =
            _detector->getMarker((uchar*)bgraImage->imageData, &_models);
    Q_UNUSED(detected);
    _engine->update();
    emit setStatus(rgbImage, bgraImage, &_models);
    _timer->setSingleShot(true);
    _timer->start(5);
}

bool Controller::initAR() {
    bool ret;
    QString camera(ARTK_DEFAULT_CAMERA);
    _detector->setParameters(SCREEN_WIDTH, SCREEN_HEIGHT, 100, camera);

    if (!loadConfig() || !_reader->initReader()) {
        goto error;
    }


    connect(_timer, SIGNAL(timeout()), this, SLOT(showImage()));
    _timer->setSingleShot(true);
    _timer->start(1);

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

void Controller::toggleDebug()
{
    foreach(Model3D *model, _models) {
        if (!model->name().contains("thermo")) {
            model->setDebug(!model->debug());
        }
    }
}

void Controller::toggleModel()
{
    foreach(Model3D *model, _models) {
        if (!model->name().contains("thermo")) {
            model->setModelVisible(!model->modelVisible());
        }
    }
}
