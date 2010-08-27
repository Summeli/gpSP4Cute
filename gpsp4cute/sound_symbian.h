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
#ifndef SOUND_SYMBIAN_H_
#define SOUND_SYMBIAN_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


//Wrapper functions for symbian audio
void Init_Symbian_Audio( int samplerate, int bytesperframe );
void DeInit_SYmbian_Audio();
void set_SymbianVolume( int volume );
void Start_Symbian_Audio( int vol );
void Stop_Symbian_Audio();

extern void sound_callback(void *userdata, u8 *stream, int length);


#ifdef __cplusplus
};
#endif

#endif /* SOUND_SYMBIAN_H_ */
