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
#include "gpspadaptation.h"

//some symbian headers
#include <e32std.h>
#include <e32base.h>

//adaptation
#include "MEmulatorAdaptation.h"
#include "cuteDebug.h"
#include "symbian_memory_handler.h"
#include "symb_adaptation.h"

//qt headers
#include <QDir>
#include <QApplication>

//gpsp stuff
#include "debug.h"
#include "common.h"
#include "main.h"
#include "memory.h"

extern MEmulatorAdaptation* g_adaption;
gpspadaptation* g_gpspAdapt;

u16* g_screenptr;
quint8 paused;

quint32 adaptationkey = 0;

/*
 * 240 x 160 resolution is used in gpsp emulator, the image is scaled in the apdatation layer
 * */
#define KSCREENPITCH 240
#define KSCREENHEIGHT 160
#define KSCREENWIDTH 240

gpspadaptation::gpspadaptation( QBlitterWidget* widget  )
    {
    blitter = widget;
    g_gpspAdapt = this;
    }

void gpspadaptation::run()
    {
    __DEBUG_IN
    
    __DEBUG2("QThread::currentThreadId():", QThread::currentThreadId());
 
    
	connect(this, SIGNAL(frameblit()), blitter, SLOT(render()), 
			Qt::BlockingQueuedConnection );
	
	__DEBUG3("Starting the gpsp, ROM and bios:", rom, gsettings.iBios );
	//Start the main in the emulator
	symbian_library_main( (u8*) rom.toStdString().c_str(), 
			(u8*) gsettings.iBios.toStdString().c_str() );
	
	__DEBUG1("Main loop returned!");
    disconnect(this, SIGNAL(frameblit()), blitter, SLOT(render()) );
    }


gpspadaptation::~gpspadaptation()
{
   //call the exit stuff for the main
}

void gpspadaptation::Start()
	{
    __DEBUG_IN
    paused = 0;
    __DEBUG_OUT
    
	}


void gpspadaptation::Stop()
	{
    __DEBUG_IN
    paused = 1;
    __DEBUG_OUT
	}

void gpspadaptation::ResetGame()
	{
    __DEBUG_IN
    reset_gba();
    reg[CHANGED_PC_STATUS] = 1;
    __DEBUG_OUT	
	}

void gpspadaptation::LoadRom( QString aFileName,  TGPSPSettings settings )
	{
	__DEBUG_IN
    gsettings = settings; 
    rom = aFileName;
    paused = 0;
    if( !isRunning() )
    	{
		__DEBUG1("thread wasn't yet running, starting the thread");
    	start( QThread::LowPriority );
    	}
    else
    	{
		//no need to start thread, let's just load new rom instead
        save_game_config_file();
		if(load_gamepak( (char*) aFileName.toStdString().c_str() ) == -1)
			DEBUG("Game load failed");
	    reset_gba();
	    reg[CHANGED_PC_STATUS] = 1;
    	}
    __DEBUG_OUT
	}

void gpspadaptation::savegpspState( int aState )
	{
    __DEBUG_IN
    saveState( aState );
    __DEBUG_OUT
	}

void gpspadaptation::loadgpspState( int aState )
	{
    __DEBUG_IN
    loadState( aState );
    __DEBUG_OUT
	}

void gpspadaptation::exitgpsp()
	{
    __DEBUG_IN
    paused = 0;
    doExitgpsp();
    __DEBUG_OUT
	}

void gpspadaptation::showgpspFPS( bool fps )
	{
	__DEBUG_IN
	gp2x_fps_debug = fps;
	__DEBUG_OUT
	}

void gpspadaptation::blit( const quint16* screen )
	{
    __DEBUG_IN
    emit(frameblit());
    __DEBUG_OUT
	}

QString gpspadaptation::gameconfigpath()
	{
	__DEBUG_IN	
	QString drive = QApplication::applicationDirPath().left(2); 
	drive.append( "\\private\\E0D278F3\\game_config.txt");
	return drive;
	}

QString gpspadaptation::getMainpath()
	{
	 QDir dir(rom);
	 return dir.absolutePath();
	}

void gpspadaptation::showErrorNote( QString message )
	{
	__DEBUG_IN
	// send the signal into the UI thread, where the dialog can
	// be launched safely
	emit(dispatchErrorNote(message));
	__DEBUG_OUT
	}

u32 updateSymbianInput()
	{
	__DEBUG_IN
	while( paused == 1)
		{
		 symb_usleep(15000);
		}
	antKeyEvent event;
	while(g_adaption->getKeyEvent(event))
		{
		if(event.keyDown )
			{
			adaptationkey |= event.key;
			}
		else
			{
			adaptationkey &= ~event.key;
			}
		}
	__DEBUG_OUT
	return adaptationkey;
	}

void symbian_blit( const u16* screen )
	{    
	__DEBUG_IN
	g_gpspAdapt->blit( (quint16*) screen );
	__DEBUG_OUT
	}

u16* allocateFramenBuffer( u32 size )
	{
	__DEBUG_IN
	//TODO: we could actually make a QImage in here, and use that pointer
	//Maybe with Symbian^3 :-)
	g_screenptr = (u16*) malloc( size );
	return g_screenptr;
	__DEBUG_OUT
	}

void freeFrameBuffer()
	{
	__DEBUG_IN
	free(g_screenptr);
	__DEBUG_OUT
	}

u16* get_screen_pixels()
	{
	//return pointer into the framebuffer
	return g_screenptr;
	}

void gameconfigFilePath( char* path )
	{
	QString qpath =  g_gpspAdapt->gameconfigpath();
	__DEBUG2("gameconfigilepath qpath= ", qpath);
	 sprintf(path, "%s", qpath.toStdString().c_str() );  
	}

void getmainPath( char* path )
	{
	QString qpath = g_gpspAdapt->getMainpath();
	__DEBUG2("mainpath= ", qpath);
	 sprintf(path, "%s", qpath.toStdString().c_str() );  
	}

void dispatchNoBiosErrorNote()
	{
	__DEBUG_IN
	QString error("\n");
	error.append("Sorry, but gpSP requires a Gameboy Advance BIOS   \n");
	error.append("image to run correctly. Make sure to get an       \n");
	error.append("authentic one, it'll be exactly 16384 bytes large \n");
	error.append("and should have the following md5sum value:       \n");
	error.append("                                                  \n");
	error.append("a860e8c0b6d573d191e4ec7db1b1e4f6                  \n");
	g_gpspAdapt->showErrorNote( error );
	__DEBUG_OUT
	}

void dispatchWrongBiosErrorNote()
	{
	__DEBUG_IN
	QString error("\n");
	error.append("You have an incorrect BIOS image.                 \n");
	error.append("While many games will work fine, some will not. It\n");
	error.append("is strongly recommended that you obtain the       \n");
	error.append("correct BIOS file. Do NOT report any bugs if you  \n");
	error.append("are seeing this message.                          \n");
	g_gpspAdapt->showErrorNote( error );
	__DEBUG_OUT
	}

void dispatchRomErrorNote()
	{
	
	}

bool isBiosValid( const char* bios )
	{
	if( load_bios( (char*) bios ) == -1 )
	  {
	  return false;
	  }
	if(bios_rom[0] != 0x18)
	  {
	  return false;
	  }
	return true;
	}

//Debug helpers for the C-side
void WriteLOG( char* x )
	{
	__DEBUG1(x);
	}
void WriteLOG1( char* x, char* v )
	{
	__DEBUG2(x,v);
	}
void WriteLOG2( char* x, char* v, char* v1 )
	{
	__DEBUG3(x,v,v1);
	}
void WriteLOG3( char* x, char* v, char* v1, char* v2 )
	{
	__DEBUG4(x,v,v1,v2);
	}
void WriteLOG4( char* x, char* v, char* v1, char* v2, char* v3 )
	{
	
	}

void InitDEBUG()
	{
	
	}
