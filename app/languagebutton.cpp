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

#include <QPainter>
#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QPaintEvent>
#include <QApplication>
#include <QLocale>
#include "languagebutton.h"

LanguageButton::LanguageButton(QWidget *parent) :
    MenuButton(parent)
{
    setBackground();
}

void LanguageButton::resizeEvent(QResizeEvent *event) {
    this->setGeometry(parentWidget()->width() - 150,
                      parentWidget()->height() - 90,
                      70, 70);

}

void LanguageButton::changeLanguage() {
    QLocale locale;
    QLocale::Language language;

    if (locale.language() == QLocale::Finnish) {
        language = QLocale::English;
    } else {
        language = QLocale::Finnish;
    }
    emit languageChanged(language);
}


void LanguageButton::setBackground() {
    QLocale locale;
    if (locale.language() == QLocale::Finnish) {
        setStyleSheet(FINNISH_FLAG);
    } else {
        setStyleSheet(ENGLISH_FLAG);
    }
}

void LanguageButton::mousePressEvent(QMouseEvent *event) {
    QLocale locale;
    if (locale.language() == QLocale::Finnish) {
        setStyleSheet(FINNISH_PRESSED);
    } else {
        setStyleSheet(ENGLISH_PRESSED);
    }
}


void LanguageButton::mouseReleaseEvent(QMouseEvent *event) {
    changeLanguage();
    setBackground();
}



