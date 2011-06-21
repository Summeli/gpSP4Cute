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

#include "gpsp4Qt.h"

#include "cuteDebug.h"
#include "symbian_memory_handler.h"  
#include "input.h"

uint KAntKeyTable[10]={BUTTON_UP,BUTTON_DOWN,BUTTON_LEFT,BUTTON_RIGHT,BUTTON_A,
		BUTTON_B,BUTTON_L,BUTTON_R,BUTTON_START,BUTTON_SELECT};

MEmulatorAdaptation* g_adaption;

gpsp4Qt::gpsp4Qt(QWidget *parent)
    : QMainWindow(parent)
{
	QThread::currentThread()->setPriority( QThread::NormalPriority );
	showFullScreen();
	
    //create button widgets
    dpad = new DPadWidget( this );
    dpad->setGeometry(QRect(0, 0, 160, 360));
    dpad->show();
    connect(dpad, SIGNAL(showMenu()), this, SLOT( showAntSnesMenu()) );
    connect(dpad, SIGNAL(virtualKeyEvent(quint32, bool)), this, SLOT( virtualKeyEvent(quint32, bool)) );
    
    smallwidget = new smalloptionswidget( this );
    smallwidget->setGeometry(QRect(0, 0, 80, 360));
    smallwidget->hide();
    connect(smallwidget, SIGNAL(showMenu()), this, SLOT( showAntSnesMenu()) );
    connect(smallwidget, SIGNAL(virtualKeyEvent(quint32, bool)), this, SLOT( virtualKeyEvent(quint32, bool)) );
    
    rsmallwidget = new rightbuttonwidget( this );
    rsmallwidget->setGeometry(QRect(560, 0, 640, 360));
    rsmallwidget->hide();
     
    widget = new QBlitterWidget( this );
    widget->setObjectName(QString::fromUtf8("QBlitterWidget"));
    widget->setGeometry(QRect(160, 0, 640, 360));
    
    m_audio = new CAntAudio();
    
    adaptation = new gpspadaptation( widget, m_audio );
    
    connect(this, SIGNAL(Start()), widget, SLOT(startDSA()) );
    connect(this, SIGNAL(Stop()), widget, SLOT(stopDSA()) );
    
    
    //connect all adaptation stuff
    connect(this, SIGNAL(Start()), adaptation, SLOT(Start()) );
    connect(this, SIGNAL(Stop()), adaptation, SLOT(Stop()) );
     
    connect(this, SIGNAL(saveState(int)), adaptation, SLOT(savegpspState(int)) );
    connect(this, SIGNAL(loadState(int)), adaptation, SLOT(loadgpspState(int)) );
    connect(this, SIGNAL(resetgpsp()), adaptation, SLOT(ResetGame()) );
    connect(this, SIGNAL(exitgpsp()), adaptation, SLOT(exitgpsp()) );
    connect(this, SIGNAL(doLoadROM(QString,TGPSPSettings)), adaptation, SLOT(LoadRom(QString,TGPSPSettings)) );
    connect(adaptation, SIGNAL(dispatchErrorNote(QString)), this, SLOT(showErrorNote(QString)) );
    
    connect(this, SIGNAL(Start()), this, SLOT(listencontrols()) );
    
    g_adaption = this;
    
    errorDialog = NULL;
}

gpsp4Qt::~gpsp4Qt()
{
	delete widget;
	delete dpad;
}

void gpsp4Qt::setRemoteControl( QRemoteControlKeys* remote )
	{
	remotecontrol = remote;
	}

void gpsp4Qt::keyPressEvent( QKeyEvent * event)
    {
	__DEBUG_IN
    __DEBUG2("Key Pressed: ", event->text()); 
    
    quint32 c = event->nativeScanCode();
    antKeyEvent keyevent;
    
	__DEBUG2("key pressed, scancode is ", c );
	for(TInt i=0;i<10;i++)
		{
		if(c==iSettings.iScanKeyTable[i])
			{
		    __DEBUG4("pressed gpsp mapped key in round:", i, "which is:", KAntKeyTable[i]);
			keyevent.key = KAntKeyTable[i];
			keyevent.keyDown = true;
			iPressedKeys.append( keyevent );
			break;
			}
		}

    }

void gpsp4Qt::keyReleaseEvent(QKeyEvent* event)
	{
	__DEBUG_IN
    quint32 c = event->nativeScanCode();
    antKeyEvent keyevent;
    
	__DEBUG2("key released, scancode is ", c );
	for(TInt i=0;i<10;i++)
		{
		if(c==iSettings.iScanKeyTable[i])
			{
			__DEBUG4("released gpsp mapped key in round:", i, "which is:", KAntKeyTable[i]);
			keyevent.key = KAntKeyTable[i];
			keyevent.keyDown = false;
			iPressedKeys.append( keyevent );
			break;
			}
		}
	__DEBUG_OUT
	}

void gpsp4Qt::LoadROM(QString rom, TGPSPSettings antSettings )
    {
	__DEBUG_IN
	iSettings = antSettings;
	currentROM = rom;
	updateSettings( antSettings );
	emit( doLoadROM( rom, antSettings ));
	emit(Start());
	__DEBUG_OUT
    }

void gpsp4Qt::showAntSnesMenu()
	{
	__DEBUG_IN
	emit( Stop() );
	emit( showmenu() );
	__DEBUG_OUT
	}

void gpsp4Qt::LoadState( int state )
	{
	__DEBUG_IN
	emit( loadState( state ) );
	__DEBUG2("state loaded", state);
	emit(Start());
	__DEBUG_OUT
	}

void gpsp4Qt::SaveState( int state )
	{
	__DEBUG_IN
	emit ( saveState( state ) );
	__DEBUG2("state saved", state);
	emit(Start());
	__DEBUG_OUT
	}

void gpsp4Qt::reset()
	{
	__DEBUG_IN
	emit( resetgpsp() );
	emit( Start() );
	__DEBUG_OUT
	}

void gpsp4Qt::continueGame()
	{
	emit( Start() );
	}

void gpsp4Qt::updateSettings( TGPSPSettings antSettings )
	{
	__DEBUG_IN
	iSettings = antSettings;
	adaptation->updateSettings( antSettings );
	widget->setScreenMode(antSettings.iScreenSettings, 
			antSettings.ikeepAspectRatio);
	
	//check d-pad
	if( antSettings.iScreenSettings == 0 )
		{
		dpad->show();
		smallwidget->hide();
		rsmallwidget->hide();
		}
	else
		{
		dpad->hide();
		smallwidget->show();
		rsmallwidget->show();
		}
	
	__DEBUG_OUT
	}

void gpsp4Qt::virtualKeyEvent( quint32 aKey, bool isDown )
	{
	__DEBUG_IN
    antKeyEvent keyevent;
    keyevent.key = aKey;
    keyevent.keyDown = isDown;
    iPressedKeys.append( keyevent );
    __DEBUG_OUT
	}

void gpsp4Qt::showErrorNote( QString message )
	{
	__DEBUG_IN
	emit(Stop());
	errorDialog = new cuteErrorDialog( message, this );
	connect(errorDialog, SIGNAL(notifiedUser()), this, SLOT(errorNoteDismissed()) );
	errorDialog->show();
	errorDialog->setFocus();
	__DEBUG_OUT
	}

void gpsp4Qt::errorNoteDismissed()
	{
	disconnect(errorDialog, SIGNAL(notifiedUser()), this, SLOT(errorNoteDismissed()) );
	delete errorDialog;
	errorDialog = NULL;
	emit(Start());
	}


int gpsp4Qt::getKeyEvent( antKeyEvent& keyEvent )
	{
	if( iPressedKeys.isEmpty() )
		{
		return 0;
		}
	keyEvent = iPressedKeys.takeFirst();
	 __DEBUG3("returning keyevent, pressed, scancode", keyEvent.keyDown, keyEvent.key );
	 return 1;
	}

void gpsp4Qt::listencontrols()
	{
	remotecontrol->subscribeKeyEvent(this);
	}

