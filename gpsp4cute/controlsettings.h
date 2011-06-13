/* gpsp4cute
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#ifndef CONTROLSETTINGS_H
#define CONTROLSETTINGS_H

#include <QtGui/QWidget>
#include "ui_controlsettings.h"

class controlsettings : public QWidget
{
    Q_OBJECT

public:
    controlsettings( int dpadSettings, int hiddenABButtons, QWidget *parent = 0);
    ~controlsettings();

public slots: 
     void keyConfig();
     void setDPadSettings( int settings );
     void setButtonSettings( int settings );
signals:
     void runkeyconfig();
     void dpadSettings(int settings);
     void buttonSettings( int settings );

private:
    Ui::controlsettingsClass ui;
};

#endif // CONTROLSETTINGS_H
