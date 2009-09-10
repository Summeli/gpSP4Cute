/* gameplayS60p
 *
 * Copyright (C) 2009 summeli AT summeli.fi
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

#ifndef __DEBUG_H__
#define __DEBUG_H__

//#define __DEBUG__ON__
//#define __DEBUG_TO_FILE__

#ifdef __DEBUG__ON__

#define DEBUG(x) WriteLOG( x )
#define DEBUG1(x, v) WriteLOG1( x, v )
#define DEBUG2(x, v1, v2) WriteLOG2( x, v1, v2 )
#define DEBUG3(x, v1, v2, v3) WriteLOG3( x, v1, v2, v3 )
#define DEBUG4(x, v1, v2, v3, v4) WriteLOG4( x, v1, v2, v3, v4 )
#define INITSYMBIANDEBUG InitDEBUG();

#else
#define DEBUG(x)
#define DEBUG1(x, v)
#define DEBUG2(x, v1, v2)
#define DEBUG2(f, s, v1, v2)
#define DEBUG3(x, v1, v2, v3)
#define DEBUG4(x, v1, v2, v3, v4)
#define INITSYMBIANDEBUG
#endif 

#ifdef __cplusplus
extern "C" {
#endif
void WriteLOG( char* x );
void WriteLOG1( char* x, char* v );
void WriteLOG2( char* x, char* v, char* v1 );
void WriteLOG3( char* x, char* v, char* v1, char* v2 );
void WriteLOG4( char* x, char* v, char* v1, char* v2, char* v3 );
void InitDEBUG();
#ifdef __cplusplus
};
#endif

#endif
