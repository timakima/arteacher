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

#ifndef DEFINES_H
#define DEFINES_H

#define APP_NAME "AR Physics Teacher"

/* TODO: create a camera selection panel */
/* especially if autodetecting image size fails */
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define MAX_CAMERA_DEVICES 2

/* TODO: create a configuration panel for these */
#define ARTK_DEFAULT_PATTERN  "patt.hiro"
#define ARTK_DEFAULT_CAMERA "logitech_c615.dat"
#define ARTK_MARKER_WIDTH 34.0
#define ARTK_CONFIDENCE 0.4
#define ARTK_THRESHOLD 100

#define GL_DEFAULT_SCALE 14.0

#define UI_BUTTONS_IN_ROW 3

#define ANIMATION_BALLS 100

#define MIN_TEMP (-20.0)
#define MAX_TEMP 100.0
#define TEMP_RADIUS 250.0
#define MIN_VELOCITY 325.0
#define MAX_VELOCITY 392.0


#define FINNISH_FLAG "border-image: url(graphics/finnish.png);"
#define FINNISH_PRESSED "border-image: url(graphics/finnish_pressed.png);"
#define ENGLISH_FLAG "border-image: url(graphics/english.png);"
#define ENGLISH_PRESSED "border-image: url(graphics/english_pressed.png);"

enum LANGUAGE {
    LANG_FI,
    LANG_EN
};

enum INFO_WIDGET {
    WIDGET_LCD,
    WIDGET_IMAGE,
    WIDGET_BOLTZMANN
};

#endif // DEFINES_H
