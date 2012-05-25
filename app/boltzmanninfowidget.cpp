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
#include <QPainter>
#include <QPen>
#include <QPaintEvent>
#include "boltzmanninfowidget.h"
#include "defines.h"
#include <math.h>

BoltzmannInfoWidget::BoltzmannInfoWidget(QWidget *parent) :
    QWidget(parent), _temp(-1), _velocity(-1)
{

}


void BoltzmannInfoWidget::drawCoords(QPainter *painter) {
    qreal topY = 10.0;
    qreal leftX = 10.0;
    qreal rightX = this->width() - leftX;
    qreal bottomY = this->height() - topY;
    QVector<QLineF> lines;
    QLineF xAxis(leftX, bottomY, rightX, bottomY);
    QLineF xArrow1(rightX, bottomY, rightX - 5, bottomY + 5);
    QLineF xArrow2(rightX, bottomY, rightX - 5, bottomY - 5);
    QLineF yAxis(leftX, topY, leftX, bottomY);
    QLineF yArrow1(leftX, topY, leftX - 5, topY + 5);
    QLineF yArrow2(leftX, topY, leftX + 5, topY + 5);
    lines.append(xAxis);
    lines.append(xArrow1);
    lines.append(xArrow2);
    lines.append(yAxis);
    lines.append(yArrow1);
    lines.append(yArrow2);
    painter->drawLines(lines);
}

void BoltzmannInfoWidget::drawLabels(QPainter *painter) {
    QString textT = "T (C)";
    QString textV = "v (m/s)";

    qreal topY = 10.0;
    qreal leftX = 10.0;
    qreal rightX = this->width() - leftX;
    qreal bottomY = this->height() - topY;

    painter->drawText((int)rightX - 50, (int)bottomY - 40, 50, 20, 0, textT);
    painter->drawText((int)leftX + 30, (int)topY + 20, 50, 20, 0, textV);

}

void BoltzmannInfoWidget::drawChart(QPainter *painter) {
    qreal topY = 10.0;
    qreal leftX = 10.0;
    qreal rightX = this->width() - leftX;
    qreal bottomY = this->height() - topY;
    QPen pen;
    pen.setColor(QColor(0x60, 0x60, 0x60));
    pen.setWidth(2);
    painter->setPen(pen);
    QLineF constant(leftX, bottomY, rightX, topY);
    painter->drawLine(constant);
}

void BoltzmannInfoWidget::drawStatus(QPainter *painter) {
    qreal topY = 10.0;
    qreal leftX = 10.0;
    qreal rightX = this->width() - leftX;
    qreal bottomY = this->height() - topY;

    qreal dx = rightX - leftX;
    qreal dy = bottomY - topY;

    qreal percentage = ((qreal)_velocity - (qreal)MIN_VELOCITY) /
            ((qreal)MAX_VELOCITY - (qreal)MIN_VELOCITY);

    qreal x = leftX + dx * percentage - 4;
    qreal y = bottomY - dy * percentage - 4;

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(8);
    painter->setPen(pen);
    painter->drawEllipse(x, y, 8, 8);

}

void BoltzmannInfoWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(QColor(0xEE, 0xEE, 0xEE));
    pen.setWidth(3);
    painter.setPen(pen);
    painter.fillRect(event->rect(), QBrush(QColor(0x30, 0x30, 0x30)));
    drawCoords(&painter);
    drawLabels(&painter);
    drawChart(&painter);
    drawStatus(&painter);
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
    this->update();
}

