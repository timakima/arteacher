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

#include <cmath>
#include "engine.h"
#include "defines.h"

Engine::Engine(QObject *parent) :
    QObject(parent), _temp(DEFAULT_TEMP), _tempDistance(100.0)
{
}


qreal Engine::distance(QMatrix4x4 &a, QMatrix4x4 &b) {
    QVector4D coordA = a.column(3);
    QVector4D coordB = b.column(3);
    QVector4D coordC;

    coordC = coordA - coordB;
    return coordC.length();
}

void Engine::setTemp(qreal temp) {
    _temp = temp;
}

qreal Engine::tempInDistance(qreal radius, qreal distance,
                             qreal envTemp,
                             qreal targetTemp) {
    qreal ret;
    qreal tempDiff;
    qreal fraction;
    if (distance > -0.01f && distance < 10.0f) {
        ret = targetTemp;
        goto done;
    }

    if (distance > radius) {
        ret = envTemp;
        goto done;
    }

    fraction = distance / _tempDistance;
    tempDiff = (targetTemp - envTemp) * fraction;
    ret = targetTemp - tempDiff;

    done:
    return ret;
}

void Engine::updateTemp(bool coordinateFix) {
    qreal particleVel;
    QVector3D up(0.0f, 0.0f, -1.0f * 5.0f  );
    QVector3D thermo(0.0f, 90.0f, 30.0f);

    QMatrix4x4 fridgeMat = _fridge->transMat();
    QMatrix4x4 stoveMat = _stove->transMat();
    QMatrix4x4 tempMat = _thermo->transMat();

    if (coordinateFix) {
        fridgeMat.translate(up);
        stoveMat.translate(up);
        tempMat.translate(thermo);
    }

    qreal distToFridge = distance(tempMat, fridgeMat);
    qreal distToStove = distance(tempMat, stoveMat);

    qreal thermoTemp = _temp;

    /* Stove, thermo, or fridge missing, can't calculate distance */
    if (std::isnan(thermoTemp)) {
        thermoTemp = DEFAULT_TEMP;
    }

    thermoTemp =
            tempInDistance(_tempDistance, distToFridge, thermoTemp,
                           _fridge->temp());
    thermoTemp =
            tempInDistance(_tempDistance, distToStove, thermoTemp,
                           _stove->temp());

    particleVel = velocity(thermoTemp);

    _thermo->setTemp(thermoTemp);
    _balls->setTemp(thermoTemp);
    _display->setTemp(thermoTemp);

    emit refreshTemp((int)thermoTemp);
    emit refreshVel((int)particleVel);
    emit refresh(thermoTemp, particleVel);
}


void Engine::updateCoords() {
    if (_balls && _thermo && _display) {
        _balls->setVisible(_thermo->visible());
        _balls->setTransMat(_thermo->transMat());
        _display->setVisible(_thermo->visible());
        _display->setTransMat(_thermo->transMat());
    }

}

void Engine::update(bool coordinateFix) {
    updateCoords();
    updateTemp(coordinateFix);
}


qreal Engine::velocity(qreal temp) {
    qreal k = (MAX_VELOCITY - MIN_VELOCITY) / (MAX_TEMP - MIN_TEMP);
    return MIN_VELOCITY + k * (temp - MIN_TEMP);
}



qreal Engine::temp() { return _temp; }

void Engine::setTempDistance(qreal tempDistance) {
    _tempDistance = tempDistance;
}

qreal Engine::tempDistance() { return _tempDistance; }

void Engine::setModels(Model3D *fridge, Model3D *stove, Model3D *thermo,
                       Model3D *balls, Model3D *display) {
    _fridge = fridge;
    _stove = stove;
    _thermo = thermo;
    _balls = balls;
    _display = display;
}

Model3D *Engine::fridge() { return _fridge; }
Model3D *Engine::stove() { return _stove; }
Model3D *Engine::thermo() { return _thermo; }
Model3D *Engine::balls() { return _balls; }
Model3D *Engine::display() { return _display; }

