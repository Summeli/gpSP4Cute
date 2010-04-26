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


#include "keyconfigdialog.h"

#include "cuteDebug.h"
//include the config file

keyconfigdialog::keyconfigdialog(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	round = 0;
}

keyconfigdialog::~keyconfigdialog()
{

}

void keyconfigdialog::keyPressEvent( QKeyEvent * event)
    {
	updateText();
	
	if( event->key() != 0 )
		{
		__DEBUG2("keyPressEvent: native scancode is:", event->nativeScanCode());
		scanKeyTable[round] = event->nativeScanCode();
		}
	
    if( round == 9 )
        emit(configDone());
    
    round++;
    }

void keyconfigdialog::cancelClicked()
	{
	updateText();
	scanKeyTable[round] = 0;
    if( round == 9 )
        emit(configDone());
	round++;

	}

quint32 keyconfigdialog::getKeyBind( int key )
	{
	return scanKeyTable[key];
	}

void keyconfigdialog::updateText()
	{
	switch( round )
		{
		case 0:
			 ui.keyLabel->setText("Press Down, or \n Press cancel to skip");
			 break;
		case 1:
			ui.keyLabel->setText("Press Left, or \n Press cancel to skip");
			break;
		case 2:
			ui.keyLabel->setText("Press Right, or \n Press cancel to skip");
			break;
		case 3:
			ui.keyLabel->setText("Press A button, or \n Press cancel to skip");
			break;
		case 4:
			ui.keyLabel->setText("Press B button, or \n Press cancel to skip");
			break;
		case 5:
			ui.keyLabel->setText("Press L button, or \n Press cancel to skip");
			break;
		case 6:
			ui.keyLabel->setText("Press R button, or \n Press cancel to skip");
			break;
		case 7:
			ui.keyLabel->setText("Press START button, or \n Press cancel to skip");
			break;
		case 8:
			ui.keyLabel->setText("Press SELECT button, or \n Press cancel to skip");
			break;
		case 9:
			break;
		}
	
	}
