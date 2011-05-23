/*
 * TheRelocator, dynamic text segment relocation routine v1.0 for SymbianOS.
 * Copyright (C) 2009,2010 Olli Hinkka
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef __RELOCUTILS__H__
#define __RELOCUTILS__H__

#ifdef __cplusplus

#include <e32std.h>

#define RELOCATED_CODE(x) TheRelocator __theRelocator((TAny*)x);

extern "C" unsigned int original_ro_base;
extern "C" int relocated_ro_offset;

NONSHARABLE_CLASS(TheRelocator)
    {
    public:
        TheRelocator(TAny* aTargetAddr);
        ~TheRelocator();
    private:
        TheRelocator();
      	void Jmp(TAny* aOldCodeBase, TAny* aNewCodeBase);
      	void Relocate(TAny* aTargetAddr);
	    TAny* iOldBaseAddr;
	    TAny* iNewBaseAddr;
	    RChunk iChunk;
    };

extern "C" void* __begin_relocated_code(void* ptr);
extern "C" void __end_relocated_code(void* ptr);

#else

extern void* __begin_relocated_code(void* ptr);
extern void __end_relocated_code(void* ptr);
extern unsigned int original_ro_base;
extern int relocated_ro_offset;

#endif

#define BEGIN_RELOCATED_CODE(x) void* __reloc_obj_ptr=__begin_relocated_code((void*)x);
#define END_RELOCATED_CODE() __end_relocated_code(__reloc_obj_ptr);

#endif
