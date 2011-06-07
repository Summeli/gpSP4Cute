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


#include "videosettings.h"

videosettings::videosettings( bool showFps, int buttonOpacity, int stretch, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.showFPSbox->setChecked( showFps );
    ui.strechSettings->setCurrentIndex( stretch );
    ui.buttonOpacity->setValue( buttonOpacity );

    connect(ui.showFPSbox, SIGNAL(stateChanged(int)), this, SLOT(showFPSChecked(int)));
    connect(ui.strechSettings, SIGNAL(currentIndexChanged(int)), this, SLOT(stretchIndexChecked(int)));
    connect(ui.buttonOpacity, SIGNAL(valueChanged(int)), this, SLOT(setOpacity(int)));

    setOpacity(buttonOpacity);
}

videosettings::~videosettings()
{

}

void videosettings::showFPSChecked( int state )
{
   emit (showFPS( (bool) state ));
}

void videosettings::stretchIndexChecked( int state )
{
   emit (stretch(  state ));
}

void videosettings::setOpacity(int slider)
{
    switch ( slider )
    {
        case 0:
            ui.buttonOpacityLabel->setText("0%");
            break;
        case 1:
            ui.buttonOpacityLabel->setText("25%");
            break;
        case 2:
            ui.buttonOpacityLabel->setText("50%");
            break;
        case 3:
            ui.buttonOpacityLabel->setText("75%");
            break;
        default:
            ui.buttonOpacityLabel->setText("100%");
            break;
    }
    
    emit(buttonOpacity(slider));
}
