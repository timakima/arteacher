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
#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "model3d.h"

/* Takes care of interaction between 3D models */
/* Contains Boltzmann constant functionality that will be later moved to
  derived class */
class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);

    /* Distance between two models */
    qreal distance(QMatrix4x4 &a, QMatrix4x4 &b);

    /* Environment temperature */
    void setTemp(qreal temp);
    qreal temp();

    /* Radius in which temperature is affected by a model */
    void setTempDistance(qreal tempDistance);
    qreal tempDistance();

    /* Calculates temperature between a thermometer and another model */
    qreal tempInDistance(qreal radius, qreal distance, qreal envTemp,
                         qreal targetTemp);


    /* Updates temperatures in models */
    void updateTemp(bool coordinateFix);

    /* Updates ball animation coordinates */
    void updateCoords();
    void update(bool coordinateFix = true);

    qreal velocity(qreal temp);

    /* Models for Boltzmann constant */
    void setModels(Model3D *fridge, Model3D *stove, Model3D *thermo,
                   Model3D *balls, Model3D *display);
    Model3D *fridge();
    Model3D *stove();
    Model3D *thermo();
    Model3D *balls();
    Model3D *display();

signals:
    void refreshTemp(int temp);
    void refreshVel(int velocity);
    void refresh(int temp, int velocity);
    
public slots:

private:
    qreal _temp;
    qreal _tempDistance;
    Model3D *_fridge;
    Model3D *_stove;
    Model3D *_thermo;
    Model3D *_balls;
    Model3D *_display;
    
};

#endif // ENGINE_H
