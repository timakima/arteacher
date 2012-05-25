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

#ifndef LANGUAGEBUTTON_H
#define LANGUAGEBUTTON_H

#include <QLocale>
#include "menubutton.h"
#include "defines.h"

/* A button to display language changing */
class LanguageButton : public MenuButton
{
    Q_OBJECT
public:
    explicit LanguageButton(QWidget *parent = 0);
    void setBackground();
    void changeLanguage();

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void languageChanged(QLocale::Language language);

public slots:
    
private:


};

#endif // LANGUAGEBUTTON_H
