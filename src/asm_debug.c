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

#include "debug.h"

/* Just an easy way to generate debugs form assembler side of the code. 
   I'm hope that there wont be that much to debug in the assembler side, 
   since Symbian phones should use ARM after all ;-)
   Use this at the asm side by making call_c_function(CallDebugFromASM) calls. */
#define DEBUGBUILDER \
void CallDebugFromASM() \
    { \
	DEBUG("ARRRGH"); \
	} \

DEBUGBUILDER;
