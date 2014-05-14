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
#include <QLCDNumber>
#include <QLabel>
#include <QHBoxLayout>
#include "lcdwidget.h"
#include "defines.h"


LCDWidget::LCDWidget(QWidget *parent) :
    QWidget(parent)
{
    const QString velocityText = "v (m/s)";
    const QString temperatureText = "T ("+QString::fromUtf8(CELSIUS_STR)+")";
    setLayout(new QHBoxLayout());
    QLCDNumber *velocity = addLCD(velocityText);
    connect(this, SIGNAL(velocityRefresh(int)),
            velocity, SLOT(display(int)));
    QLCDNumber *temperature = addLCD(temperatureText);
    connect(this, SIGNAL(tempRefresh(int)),
            temperature, SLOT(display(int)));
}


QLCDNumber *LCDWidget::addLCD(QString labelText) {
    const int fontSize = 20;
    const int marginSize = 0;

    QWidget *widget = new QWidget();
    QLabel *label = new QLabel(labelText);
    QLCDNumber *lcd = new QLCDNumber();
    lcd->setSegmentStyle(QLCDNumber::Flat);
    lcd->setFrameStyle(QFrame::NoFrame);
    QFont font;
    label->setAlignment(Qt::AlignVCenter);
    font.setPointSize(fontSize);
    label->setFont(font);
    QHBoxLayout *subLayout = new QHBoxLayout();
    subLayout->addWidget(lcd);
    subLayout->addWidget(label);
    subLayout->setMargin(marginSize);
    widget->setLayout(subLayout);
    layout()->addWidget(widget);
    return lcd;
}


void LCDWidget::updateLCD(int temp, int velocity)
{
    emit tempRefresh(temp);
    emit velocityRefresh(velocity);
}
