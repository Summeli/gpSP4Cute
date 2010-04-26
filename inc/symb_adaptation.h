/* AntSnes
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
#ifndef SYMB_ADAPTATION_H_
#define SYMB_ADAPTATION_H_

#include "common.h"

typedef long long int64;

#ifdef __cplusplus
extern "C" {
#endif

/*This interface should be build in the application side
 * the gpsp is just statically linking into these functions
 * for integration
 * */
u32 updateSymbianInput();
void symbian_blit( const u16* screen );

void gameconfigFilePath( char* path );
void getmainPath( char* path );

void dispatchNoBiosErrorNote();
void dispatchWrongBiosErrorNote();
void dispatchRomErrorNote();

u16* get_screen_pixels();

u16* allocateFramenBuffer( u32 size );

/*
 * This stuff is used from the C++ side to C side
 * */
int symbian_library_main(u8* rom, u8* bios );

#ifdef __cplusplus
};
#endif

#endif /* SYMB_ADAPTATION_H_ */
