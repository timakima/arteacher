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

#ifndef LANGUAGESELECTOR_H
#define LANGUAGESELECTOR_H

#include <QLocale>
#include <QObject>
#include <QTranslator>
#include "defines.h"

class LanguageSelector : public QObject
{
    Q_OBJECT
public:
    explicit LanguageSelector(QObject *parent = 0);
    ~LanguageSelector();

signals:

public slots:

    /* Changes application language */
    void setLanguage(QLocale::Language language);


private:
    QTranslator *_fi;
    QTranslator *_en;
    QTranslator *_activeTranslator;

};

#endif // LANGUAGESELECTOR_H
