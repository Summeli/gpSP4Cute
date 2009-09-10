/* gpSP4Symbian
 *
 * Copyright (C) 2009 Summeli <summeli@summeli.fi>
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

#include <eikapp.h>
#include <e32base.h>
#include <sdlapp.h>
#include <stddef.h>
#include <string.h>
#include <eikappui.h>
#include <eikenv.h>
#include <reent.h>
#include <bautils.h>
#include <stdlib.h>
#include <sdl.h>
//#include "gp2x.h"
static char KDataDir[256] = "e:\\Gba\\";

struct SymbianDirent {
	TUint32 d_fileno;		/* file number of entry */
	TUint16 d_reclen;		/* length of this record */
	TUint8  d_type; 		/* file type, see below */
	TUint8  d_namlen;		/* length of string in d_name */
	char	d_name[255 + 1];	/* name must be no longer than this */
};

/*
 * File types
 */
#define	DT_UNKNOWN	 0
#define	DT_FIFO		 1
#define	DT_CHR		 2
#define	DT_DIR		 4
#define	DT_BLK		 6
#define	DT_REG		 8
#define	DT_LNK		10
#define	DT_SOCK		12
#define	DT_WHT		14

class CGPS60PApp: public CSDLApp {
public:
	CGPS60PApp();
   	~CGPS60PApp();
#ifdef UIQ3	
	/**
	 * Returns the resource id to be used to declare the views supported by this UIQ3 app
 	 * @return TInt, resource id
   	*/
	TInt ViewResourceId();
#endif	
	TUid AppDllUid() const;

	/**
	 * This has a default empty implementation.
	 * Is called just before SDL_Main is called to allow init of system vars
	 */
	void PreInitializeAppL()
	{
		KDataDir[0] = BitmapStoreName()[0];
	}
};

#ifndef S60
CApaApplication* NewApplication() {
	// Return pointer to newly created CQMApp
	return new CGPS60PApp;
}

#include <eikstart.h>
// E32Main() contains the program's start up code, the entry point for an EXE.
GLDEF_C TInt E32Main() {
 	return EikStart::RunApplication(NewApplication);
}

#endif
CGPS60PApp::CGPS60PApp() {
}

CGPS60PApp::~CGPS60PApp() {
	CloseSTDLIB();
}

#ifdef UIQ3
#include <UAE4All.rsg>
/**
 * Returns the resource id to be used to declare the views supported by this UIQ3 app
 * @return TInt, resource id
 */
TInt CGPS60PApp::ViewResourceId() {
	return R_SDL_VIEW_UI_CONFIGURATIONS;
}
#endif
/**
 *   Responsible for returning the unique UID of this application
 * @return unique UID for this application in a TUid
 **/
TUid CGPS60PApp::AppDllUid() const {
	return TUid::Uid(0xE0D278F3);
}
