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

#include "viewcontroller.h"
#include "cuteDebug.h"

// Needed Symbian specific headers
#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#include <e32keys.h>
#endif

ViewController::ViewController()
{

#if defined(Q_OS_SYMBIAN)
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
    TRAPD(error, 
    if (appUi) {
     // Lock application orientation into landscape
     appUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape);
     appUi->SetKeyEventFlags(CAknAppUi::EDisableSendKeyShort|CAknAppUi::EDisableSendKeyLong);

    }
    );
    
    iMenuKeyHandle = CEikonEnv::Static()->RootWin().CaptureKeyUpAndDowns(EStdKeyMenu, 0, 0); 
    iMenuKeyHandle2 = CEikonEnv::Static()->RootWin().CaptureKeyUpAndDowns(EStdKeyApplication0, 0, 0); 
    iNoKeyHandle = CEikonEnv::Static()->RootWin().CaptureKeyUpAndDowns(EStdKeyNo, 0, 0);
    iNoKeyHandle2 = CEikonEnv::Static()->RootWin().CaptureKey( EKeyNo, 0, 0 );  
    iCameraKeyHandle = CEikonEnv::Static()->RootWin().CaptureKey( EKeyDevice7, 0, 0 );

#endif
    
    remotekeys = new QRemoteControlKeys();
	 
    emuView = new gpsp4Qt();
    //emuView->setGeometry(QRect(0, 0, 640, 360));
    emuView->setRemoteControl( remotekeys );
    emuView->hide();
    
    settingsView = new EmuSettings();
    settingsView->setRemoteControl( remotekeys );
    settingsView->show();
    
    connect(settingsView, SIGNAL(LoadROM( QString,TGPSPSettings)), this, SLOT(loadROM( QString,TGPSPSettings)));
    
    connect(settingsView, SIGNAL(LoadState( int )), this, SLOT(LoadState( int )));
    connect(settingsView, SIGNAL(SaveState( int )), this, SLOT(SaveState( int )));
    connect(settingsView, SIGNAL(resetGame()), this, SLOT(reset()));
    connect(settingsView, SIGNAL(continueGame()), this, SLOT(continueGame()));
    
    connect(settingsView, SIGNAL(updateSettings(TGPSPSettings)), emuView, SLOT(updateSettings(TGPSPSettings)));
    
    connect(emuView, SIGNAL(showmenu()), this, SLOT(showSettings()));
}

ViewController::~ViewController()
{
	delete remotekeys;
	delete settingsView;
	delete emuView;
#if defined(Q_OS_SYMBIAN)
	CEikonEnv::Static()->RootWin().CancelCaptureKeyUpAndDowns(iMenuKeyHandle);
	CEikonEnv::Static()->RootWin().CancelCaptureKeyUpAndDowns(iMenuKeyHandle2);
	CEikonEnv::Static()->RootWin().CancelCaptureKeyUpAndDowns(iNoKeyHandle);
	CEikonEnv::Static()->RootWin().CancelCaptureKey(iCameraKeyHandle);
	CEikonEnv::Static()->RootWin().CancelCaptureKey(iNoKeyHandle2);
#endif
}


void ViewController::loadROM( QString rom, TGPSPSettings antSettings )
    {
    __DEBUG_IN

    settingsView->hide();
    emuView->show();
    
    emuView->LoadROM( rom, antSettings );
    
    __DEBUG_OUT
    }

void ViewController::showSettings()
	{
	emuView->hide();
	settingsView->show();
	}

void ViewController::LoadState( int state )
	{
	__DEBUG_IN
	settingsView->hide();
	emuView->show();
	
	emuView->LoadState( state );
	__DEBUG_OUT
	}

void ViewController::SaveState( int state )
	{
	__DEBUG_IN
	settingsView->hide();
	emuView->show();
	
	emuView->SaveState( state );
	__DEBUG_OUT
	}

void ViewController::reset()
	{
	__DEBUG_IN
	settingsView->hide();
	emuView->show();
	
	emuView->reset();
	__DEBUG_OUT
	}

void ViewController::continueGame()
	{
	__DEBUG_IN
	settingsView->hide();
	emuView->show();
	
	emuView->continueGame();
	__DEBUG_OUT
	}
