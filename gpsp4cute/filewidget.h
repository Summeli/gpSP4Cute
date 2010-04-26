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

#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QtGui/QWidget>
#include "ui_filewidget.h"

class filewidget : public QWidget
{
    Q_OBJECT

public:
    filewidget(QString rom, QString bios, QWidget *parent = 0);
    ~filewidget();

public:
    void setRomLabel(QString rom);
    void setBiosLabel(QString bios);
    
public slots:
    void loadbuttonclicked();
    void setbiosclicked();
    
signals:
	void loadROM();
	void setBios();
	
private:
    Ui::filewidgetClass ui;
};

#endif // FILEWIDGET_H
