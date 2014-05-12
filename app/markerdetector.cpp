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

#include <QString>
#include <QDebug>
#include <QDir>
#include "defines.h"
#include "markerdetector.h"


MarkerDetector::MarkerDetector(){
}

MarkerDetector::~MarkerDetector() {

}

void MarkerDetector::setParameters() {
}

void MarkerDetector::setParameters(int screenWidth, int screenHeight,
                   int threshold, QString& camera) {

    ARParam wparam;
    ARParam cparam;

    _threshold = threshold;
    if (arParamLoad(camera.toLocal8Bit(), 1, &wparam) < 0 ) {
        qDebug() << "Error initializing camera parameters";
    }
    arParamChangeSize( &wparam, screenWidth, screenHeight, &cparam );
    arInitCparam(&cparam );
    arParamDisp(&cparam );
}

int MarkerDetector::findBestMatchId(ARMarkerInfo *markerInfo, int markerId, int markerNum)
{
    int bestMatch = -1;
    for (int j = 0; j < markerNum; j++) {
        if (markerInfo[j].id == markerId) {
            if (bestMatch == -1) {
                bestMatch = j;
            } else if (markerInfo[bestMatch].cf < markerInfo[j].cf) {
                bestMatch = j;
            }
        }
    }
    return bestMatch;
}

int MarkerDetector::getMarker(uchar *image, QList<Model3D*> *models)
{
    ARMarkerInfo *markerInfo;
    int markerNum = 0;
    QListIterator<Model3D*> i(*models);
    int bestMatch;

    if (!image) {
        goto done;
    }

    /* Find markers */
    if(arDetectMarker((ARUint8*)image, _threshold,
                      &markerInfo, &markerNum) < 0 ) {
        qDebug() << "Error with ARToolkit";
        goto error;
    }

    /* Iterate through models to see if they match with found markers */
    foreach(Model3D *model, *models) {

        /* Model is not registered, skip */
        if (model->markerIds().size() == 0) {
            continue;
        }

        foreach(int markerId, model->markerIds()) {

            coordinates_t *coords = model->coordPtr();
            /* Find the best match according to confidence values */
            bestMatch = findBestMatchId(markerInfo, markerId, markerNum);

            /* Not found */
            if (bestMatch == -1) {
                model->setVisible(false, markerId);
                continue;
            }

            /* Found, but wasn't visible before */
            if(!model->visible(markerId)) {
                arGetTransMat(&markerInfo[bestMatch],
                              coords->markerCenter, coords->markerWidth,
                              coords->trans);
            } else {
                /* Was visible last time, so use history to calculate
               coordinate info (reduces shaking) */
                arGetTransMatCont(&markerInfo[bestMatch], coords->trans,
                                  coords->markerCenter, coords->markerWidth,
                                  coords->trans);
            }
            qDebug() << "found" << markerId << bestMatch;
            model->updateTransMat(coords->trans, markerId);
            model->setVisible(true, markerId);

            /* Copy vertex coord info from marker to model */
            memcpy(coords->vertex, markerInfo[bestMatch].vertex,
                   8 * sizeof(double));

            //qDebug() << "found: " << model->name();

        }
    }
    //qDebug() << "marker num " << markerNum;
    goto done;

    error:
    markerNum = -1;

    done:
    return markerNum;
}
