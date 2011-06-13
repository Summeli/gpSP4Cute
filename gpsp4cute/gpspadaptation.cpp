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
#include "common.h"
//#include "memory.h"

extern MEmulatorAdaptation* g_adaption;
gpspadaptation* g_gpspAdapt;

u16* g_screenptr;
quint8 paused;

quint32 adaptationkey = 0;
bool g_audioOn;

extern "C" {

void WriteLOG( char* x );
void WriteLOG1( char* x, char* v );
void WriteLOG2( char* x, char* v, char* v1 );
void WriteLOG3( char* x, char* v, char* v1, char* v2 );
void WriteLOG4( char* x, char* v, char* v1, char* v2, char* v3 );
void InitDEBUG();

void trigger_key(u32 key);
void saveState( u32 slot );
void loadState( u32 slot );
void doExitgpsp();
void mixAudio();
int sound_callback(void *userdata, u8 *stream, int length);
};

extern u32 gp2x_fps_debug;
/*
 * 240 x 160 resolution is used in gpsp emulator, the image is scaled in the apdatation layer
 * */
#define KSCREENPITCH 240
#define KSCREENHEIGHT 160
#define KSCREENWIDTH 240

CAntAudio* g_audio;
gpspadaptation::gpspadaptation( gpsp4Qt* widget, CAntAudio* audioInterface  )
{
    m_blitter = widget;
    m_audio = audioInterface;
    g_audio = audioInterface;
    g_audioOn = false;
    g_gpspAdapt = this;
}

void gpspadaptation::run()
{
    __DEBUG_IN

    __DEBUG2("QThread::currentThreadId():", QThread::currentThreadId());

    connect(this, SIGNAL(frameblit()), m_blitter, SLOT(render()),
        Qt::BlockingQueuedConnection );

    connect(this, SIGNAL(audioFrameReady()), m_audio, SLOT(FrameMixed()), Qt::BlockingQueuedConnection);

    __DEBUG3("Starting the gpsp, ROM and bios:", m_rom, m_settings.iBios );
    //Start the main in the emulator
    symbian_library_main( (u8*) m_rom.toStdString().c_str(),
        (u8*) m_settings.iBios.toStdString().c_str() );

    __DEBUG1("Main loop returned!");
    disconnect(this, SIGNAL(frameblit()), m_blitter, SLOT(render()) );
    disconnect(this, SIGNAL(audioFrameReady()), m_audio, SLOT(FrameMixed()));
}


gpspadaptation::~gpspadaptation()
{
   //call the exit stuff for the main
}

void gpspadaptation::Start()
{
    __DEBUG_IN
    paused = 0;

    //we don't want to start this thread again
    if (!isRunning())
        start(QThread::NormalPriority);

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
    m_settings = settings;
    m_rom = aFileName;
    paused = 0;
    //m_audio->setVolume( settings.iVolume );
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
                    __DEBUG1("Game load failed");
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

void gpspadaptation::updateSettings( TGPSPSettings settings  )
{
    __DEBUG_IN
    m_settings = settings;
    gp2x_fps_debug = settings.iShowFPS;
    m_audio->setVolume( settings.iVolume );
    g_audioOn = settings.iAudioOn;
    m_audio->Reset();
    __DEBUG_OUT
}

void gpspadaptation::blit( const quint16* screen )
{
    __DEBUG_IN
    emit(frameblit());
    __DEBUG_OUT
}

void gpspadaptation::audioFrameMixed()
{
    emit audioFrameReady();
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
    QDir dir(m_rom);
    QString abspath = dir.absolutePath();
    return abspath; //TODO: rip the filename away
    //int end = abspath.indexOf("//" );
    //return abspath.left( end + 1);
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

    return g_adaption->getGpspKeys();
    __DEBUG_OUT
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
    g_screenptr = (u16*) malloc( 240*160*4 );
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
    __DEBUG_IN
    QString qpath = g_gpspAdapt->getMainpath();
    __DEBUG2("mainpath= ", qpath);
    sprintf(path, "%s", qpath.toStdString().c_str() );
    __DEBUG_OUT
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

void mixAudio( )
{
    if( g_audioOn )
      {
      u8* aframe = (u8*) g_audio->NextFrameL();
      //TODO Modify sound_callback to send if we can/can't render the audio
      if( aframe )
        {
        if ( sound_callback( NULL, aframe, KFramesize ) )
            {
            RDebug::Printf( "callback succeed, mixing" );
            g_gpspAdapt->audioFrameMixed();
            }
        }
      }
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
