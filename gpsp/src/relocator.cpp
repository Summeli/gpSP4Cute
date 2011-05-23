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


#include "relocutils.h"

unsigned int original_ro_base = 0;
int relocated_ro_offset = 0;

_LIT(KRelocatorPanic, "RELOCATOR PANIC");

TheRelocator::TheRelocator()
    {
    }

void TheRelocator::Relocate(TAny* aTargetAddr)
    {
    TProcessMemoryInfo procInfo;
    RProcess myself;
    myself.GetMemoryInfo(procInfo);
    original_ro_base = procInfo.iCodeBase;
    TInt err = iChunk.CreateLocalCode(procInfo.iCodeSize,procInfo.iCodeSize);
    if(err)
    	User::Panic(KRelocatorPanic, err);
    
    if ((TUint32)iChunk.Base() != (TUint32)aTargetAddr)
        {
        RChunk temp;
        TUint32 offset = 0;
        offset = (TUint32)aTargetAddr-(TUint32)iChunk.Base();
        iChunk.Close();
        temp.CreateLocal(0,offset);
        err = iChunk.CreateLocalCode(procInfo.iCodeSize,procInfo.iCodeSize);
        if( err )
        	User::Panic(KRelocatorPanic, err);
        temp.Close();
        }
    relocated_ro_offset=-(original_ro_base-(unsigned int)iChunk.Base());
    Mem::Copy(iChunk.Base(), (TAny*)procInfo.iCodeBase, procInfo.iCodeSize);
    User::IMB_Range(iChunk.Base(), iChunk.Base()+procInfo.iCodeSize);
    iOldBaseAddr = (TAny*)procInfo.iCodeBase;
    iNewBaseAddr = (TAny*)iChunk.Base();
    myself.Close();
    }


