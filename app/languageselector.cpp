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
#include <QApplication>
#include <QDebug>
#include <QLocale>
#include "languageselector.h"

LanguageSelector::LanguageSelector(QObject *parent) :
    QObject(parent),
    _fi(new QTranslator()), _en(new QTranslator())
{
    _fi->load("areality_fi");
    _en->load("areality_en");
    if (QLocale::system().language() == QLocale::Finnish) {
        setLanguage(QLocale::Finnish);
    } else {
        setLanguage(QLocale::English);
    }
}

LanguageSelector::~LanguageSelector() {
    if (_activeTranslator) {
        qApp->removeTranslator(_activeTranslator);
    }
    delete _fi;
    delete _en;
}

void LanguageSelector::setLanguage(QLocale::Language language) {

    if (_activeTranslator) {
        qApp->removeTranslator(_activeTranslator);
    }

    switch(language) {
    case QLocale::Finnish:
        QLocale::setDefault(QLocale(QLocale::Finnish, QLocale::Finland));
        _activeTranslator = _fi;
        break;
    case QLocale::English:
        QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
        _activeTranslator = _en;
        break;
    default:
        _activeTranslator = _en;
        break;
    }
    qApp->installTranslator(_activeTranslator);
}
