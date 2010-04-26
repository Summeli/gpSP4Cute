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


#include "filewidget.h"

filewidget::filewidget(QString rom, QString bios, QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.romLabel->setText( rom );
	ui.biosLabel->setText( bios );
	connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadbuttonclicked()));
	connect(ui.biosButton, SIGNAL(clicked()), this, SLOT(setbiosclicked()));
	
}

filewidget::~filewidget()
{

}

void filewidget::setRomLabel(QString rom)
	{
	ui.romLabel->setText( rom );
	}

void filewidget::setBiosLabel(QString bios)
	{
	ui.biosLabel->setText( bios );
	}

void filewidget::loadbuttonclicked()
	{
	emit(loadROM());
	}

void filewidget::setbiosclicked()
	{
	emit(setBios());
	}
