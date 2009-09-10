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
#ifdef __SYMBIAN32__

#include <e32std.h>

#include "symbian_memory_handler.h"
#include "cpu.h"
#include "debug.h"

static RChunk* g_code_chunk = NULL;
static RHeap* g_code_heap = NULL;


extern u8* rom_translation_cache;
extern u8* ram_translation_cache;
extern u8* bios_translation_cache;

extern u8* rom_translation_ptr;
extern u8* ram_translation_ptr;
extern u8* bios_translation_ptr;

#define FUNCTION_STUBS_SIZE (20 * 1024 )
extern u8* function_stubs_base_ptr;
extern u8* function_stubs_ptr;
//in Symbian we have to tell to the OS that these memoryblocks contains codes,
//so we can invalidate and execute the codeblock in the future.
typedef unsigned char byte; 

TInt user_counter =0;

int create_all_translation_caches()
	{
	TInt minsize = ROM_TRANSLATION_CACHE_SIZE + RAM_TRANSLATION_CACHE_SIZE + BIOS_TRANSLATION_CACHE_SIZE + FUNCTION_STUBS_SIZE;
	TInt maxsize = ROM_TRANSLATION_CACHE_SIZE + RAM_TRANSLATION_CACHE_SIZE + BIOS_TRANSLATION_CACHE_SIZE + FUNCTION_STUBS_SIZE + 3 * 4096;
	
	g_code_chunk = new RChunk();
	
	int err = g_code_chunk->CreateLocalCode(minsize, maxsize );
	if( err )
		return err;
	
    DEBUG("CREATING HEAPS");
	g_code_heap = UserHeap::ChunkHeap(*g_code_chunk, minsize, 1, maxsize );
	if( g_code_heap != NULL )
	    {
	    DEBUG("ROM HEAP SUCCESS!");
	    rom_translation_cache = (u8*) g_code_heap->Alloc( ROM_TRANSLATION_CACHE_SIZE );
	    ram_translation_cache = (u8*) g_code_heap->Alloc( RAM_TRANSLATION_CACHE_SIZE );
	    bios_translation_cache = (u8*) g_code_heap->Alloc( BIOS_TRANSLATION_CACHE_SIZE );
	    function_stubs_base_ptr = (u8*) g_code_heap->Alloc( FUNCTION_STUBS_SIZE );
	    if( rom_translation_cache == NULL)
	        DEBUG("ROM ALLOC FAIL!");
	    if( ram_translation_cache == NULL)
	        DEBUG("RAM ALLOC FAIL!");
	    if( bios_translation_cache == NULL)
	        DEBUG("BIOS ALLOC FAIL!");
	    }
	
	DEBUG("EVERYTHING DONE IN MEMORY HANDLING");
	
	rom_translation_ptr = rom_translation_cache;
	ram_translation_ptr = ram_translation_cache;
    bios_translation_ptr = bios_translation_cache;
    function_stubs_ptr = function_stubs_base_ptr;
    return 0;
	}


void CLEAR_INSN_CACHE(const u8 *code, size_t size)
{
    TUint start = (TUint) code;
	User::IMB_Range((byte*) code, (byte*)start+size);   
}

void SymbianPackHeap()
	{
	User::CompressAllHeaps();
	User::Heap().Compress();
	
	RProcess myProcess;
	myProcess.SetPriority(EPriorityForeground);
	myProcess.Close();
	}

void close_all_caches()
    {
    g_code_heap->Free( rom_translation_cache );
    g_code_heap->Free( ram_translation_cache );
    g_code_heap->Free( bios_translation_cache );
    g_code_heap->Free( function_stubs_base_ptr );
    
    g_code_heap->Close();
 

    g_code_chunk->Close();;
    }

void keepBacklightOn()
	{
	user_counter++;
	if( user_counter == 200)
		{
		User::ResetInactivityTime();
		user_counter = 0;
		}
	}

void symb_usleep(int aValue)
{
	User::AfterHighRes(aValue);
}

extern u16 interptable_w[240];
extern u16 interptable_h[160];
void symb_create_interpolate_table()
	{
	TReal j = 1.33;
	TReal loop = 0;
	TReal real_temp;
	
	for( TInt i=0; i<240; i++)
		{
		real_temp = i*j + 0.5;
		interptable_w[i] = real_temp;
		}
	
	j= 1.5;
	for( TInt i=0; i<160; i++)
		{
		real_temp = i*j + 0.5;
		interptable_h[i] = real_temp;
		}
	}
#endif
