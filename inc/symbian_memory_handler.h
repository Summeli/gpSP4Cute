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

#ifndef SYMBIAN_MEMORY_HANDLER_H_
#define SYMBIAN_MEMORY_HANDLER_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

int create_all_translation_caches();
void CLEAR_INSN_CACHE(const u8 *code, int size);
void SymbianPackHeap();
void close_all_caches();
void keepBacklightOn();
void symb_usleep(int aValue);

void symb_create_interpolate_table();

#ifdef __cplusplus
};
#endif

#endif /* SYMBIAN_MEMORY_HANDLER_H_ */
