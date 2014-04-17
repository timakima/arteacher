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

#ifndef MARKERDETECTOR_H
#define MARKERDETECTOR_H

#include <QList>
#include <QString>
#include <AR/ar.h>
#include <AR/arMulti.h>
#include "defines.h"
#include "model3d.h"

/* Frontend to ARToolkit marker detection */
class MarkerDetector
{
public:
    MarkerDetector();
    ~MarkerDetector();

public:
    /* Default parameters */
    void setParameters();

    /* Custom parameters */
    void setParameters(int screenWidth, int screenHeight, int threshold,
                       QString &camera);

    /* Finds markers from image and updates visibility status to corresponding
      models */
    int getMarker(uchar *image, QList<Model3D *> *models);

private:
    int findBestMatchId(ARMarkerInfo *markerInfo, int markerId, int markerNum);

    int _threshold;


};

#endif
