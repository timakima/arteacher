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
#include <QPainter>
#include <QPen>
#include <QPaintEvent>
#include "boltzmanninfowidget.h"
#include "defines.h"
#include <math.h>

/* Pixels from left & bottom of grid */
const int LEFTMARGIN = 50;
const int RIGHTMARGIN = 50;
const int TOPMARGIN = 50;
const int BOTTOMMARGIN = 50;
const int GRIDX = 6;
const int GRIDY = 4;
const int SCALE = 20; /* Scale for values in grid */
const float MINT = -20.0;
const float MAXT = 100.0;
const float MINV = 320;
const float MAXV = 400;
const QColor BGRCOLOR = QColor(0x30, 0x30, 0x30);

BoltzmannInfoWidget::BoltzmannInfoWidget(QWidget *parent) :
    QWidget(parent), _temp(-1), _velocity(-1)
{

}


void BoltzmannInfoWidget::drawCoords(QPainter *painter) {
    if (!painter) {
        return;
    }
    const qreal textMargin = 10.0;
    QVector<QLineF> lines;
    qreal topY = TOPMARGIN;
    qreal leftX = LEFTMARGIN;
    qreal rightX = width() - leftX;
    qreal bottomY = height() - BOTTOMMARGIN;
    qreal diffY = (bottomY - topY) / GRIDY;
    qreal diffX = (rightX - leftX) / GRIDX;
    int valueY = 400; /* Y-axis start value */
    int valueX = -20; /* X-axis start value */
    const int fontSize = 10;
    const int spacer = 10; /* px */
    const int gridWidth = 1; /* px */

    /* X-grid*/
    for (int i = 0; i <= GRIDX; i++) {
        lines.append(QLineF(leftX + (i * diffX),
                            topY, leftX + (i * diffX), bottomY));
    }

    /* Y-grid */
    for (int i = 0; i <= GRIDY; i++) {
        lines.append(QLineF(leftX, topY + (i * diffY),
                            rightX, topY + (i * diffY)));
    }

    QPen pen(Qt::gray);
    pen.setWidth(gridWidth);
    painter->setPen(pen);
    painter->drawLines(lines);

    QFont font;
    font.setPointSize(fontSize);
    painter->setFont(font);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawText(width() - RIGHTMARGIN + spacer, height() - BOTTOMMARGIN,
                      "T ("+QString::fromUtf8(CELSIUS_STR)+")");
    for (int i = 0; i <= GRIDX; i++) {
        QString text = QString::number(valueX);
        valueX += SCALE;
        painter->drawText(leftX + (i * diffX), height() - BOTTOMMARGIN/2, text);
    }



    /* Y-grid values */
    painter->drawText(0, TOPMARGIN/2, "v (m/s)");
    for (int i = 0; i <= GRIDY; i++) {
        QString text = QString::number(valueY);
        valueY -= SCALE;
        painter->drawText(textMargin, topY + (i * diffY), text);
    }


}

void BoltzmannInfoWidget::drawChart(QPainter *painter) {
    if (!painter) {
        return;
    }

    /* Pen sizes */
    const int pointSize = 8;
    const int slopeSize = 2;

    QPen slopePen;
    QPen statusPen;

    /* coord area */
    qreal topY = TOPMARGIN;
    qreal leftX = LEFTMARGIN;
    qreal rightX = width() - RIGHTMARGIN;
    qreal bottomY = height() - BOTTOMMARGIN;

    /* draw area */
    float k = (MAX_VELOCITY - MIN_VELOCITY) / (MAX_TEMP - MIN_TEMP);
    float dx = (rightX - leftX ) / (MAXT - MINT);
    float dy = (bottomY - topY) / (MAXV - MINV);

    float minVg = MAX_VELOCITY - k * (MAXT - MINT);
    float maxVg = MAX_VELOCITY;

    float minY = bottomY - (minVg - MINV) * dy;
    float maxY = bottomY - (maxVg - MINV) * dy;
    float minX = leftX;
    float maxX = leftX + (MAXT - MINT) * dx;

    qreal percentageX = (_temp - MINT) / (MAXT - MINT);

    qreal x = leftX + percentageX * (MAXT - MINT) * dx;
    qreal y = minY -  percentageX * (maxVg - minVg) * dy;

    slopePen.setColor(Qt::white);
    slopePen.setWidth(slopeSize);
    statusPen.setColor(Qt::red);
    statusPen.setWidth(pointSize);

    /* Draw the white slope */
    painter->setPen(slopePen);
    QLineF constant(minX, minY, maxX, maxY);
    painter->drawLine(constant);

    /* Draw the red dot */
    painter->setPen(statusPen);
    painter->drawEllipse(x - pointSize/2, y - pointSize/2,
                         pointSize, pointSize);

}

void BoltzmannInfoWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), QBrush(BGRCOLOR));
    drawCoords(&painter);
    painter.setRenderHint(QPainter::Antialiasing);
    drawChart(&painter);
}


void BoltzmannInfoWidget::setTemp(int temp) {
    _temp = temp;
}

void BoltzmannInfoWidget::setVel(int velocity) {
    _velocity = velocity;
}

void BoltzmannInfoWidget::refresh(int temp, int velocity) {
    setTemp(temp);
    setVel(velocity);
    update();
}

