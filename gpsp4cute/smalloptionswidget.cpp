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

#include "smalloptionswidget.h"

#include "cutedebug.h"
#include "input.h"

#include <QMouseEvent>
#include <qmath.h>

smalloptionswidget::smalloptionswidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

smalloptionswidget::~smalloptionswidget()
{

}


void smalloptionswidget::mousePressEvent(QMouseEvent* event )
	{
	__DEBUG_IN
	processbuttons( event );
	__DEBUG_OUT
	}

void smalloptionswidget::mouseReleaseEvent(QMouseEvent* event )
	{
	__DEBUG_IN
	 if( event->y() < 320 )
		 {
		 emit(virtualKeyEvent(prevkeys,false));
	     prevkeys = 0;
	     }
	__DEBUG_OUT
	}

void smalloptionswidget::mouseMoveEvent(QMouseEvent* event)
	{
	__DEBUG_IN
	processbuttons( event );
	__DEBUG_OUT
	}

void smalloptionswidget::processbuttons( QMouseEvent* event )
	{
	__DEBUG_IN
	if( event->y() > 320 )
		{
		emit(showMenu() );
		return;
		}
	else
		{
		quint32 key = getSnesKeys( event );
		if( key != prevkeys )
			{
			//release old keys
			quint32 release = prevkeys;
			release &= ~(key & prevkeys);
			emit(virtualKeyEvent(release,false));
			
			//send new key
			quint32 newkey = key;
			newkey &=  ~(key & prevkeys);
			emit(virtualKeyEvent(newkey,true));
			}
		prevkeys = key;
		}
	__DEBUG_OUT
	}

quint32 smalloptionswidget::getSnesKeys( QMouseEvent* event )
	{
	quint32 key = 0;
	quint32 ycoord = event->y();
	if(  ycoord > 280 && ycoord <= 320 )
		{
		//select was pressed
		key = BUTTON_SELECT;
		}
	else if(  ycoord > 240 && ycoord <= 280 )
        {
        //start was pressed
		key = BUTTON_START;
        }
	
	return key;
	}


