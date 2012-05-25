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


#ifndef TOOL_H
#define TOOL_H

#include <QDebug>

#define rowN QString::number(i++).toAscii

/* qFuzzyCompare seems to be too strict */
bool fuzzyCompare(qreal a, qreal b) {
    bool ret = fabs(a - b) < 0.01;
    if (!ret) qDebug() << __FUNCTION__ << ":" << a << "!=" << b;
    return ret;
}


#endif // TOOL_H
