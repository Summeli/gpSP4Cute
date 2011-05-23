#
#   Copyright 2009, Harry Li <harry.li AT pagefreedom.org>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

VARIANT = $(EPOCROOT2)/include/variant
OS_HRH = $(VARIANT)/Symbian_OS.hrh

SYMBIAN_MACRO = -D__SYMBIAN32__ -D__EPOC32__
GCCE_MACRO = -D__GCCE__ -D__MARM__ -D__MARM_ARMV5__ -D__EABI__ -march=armv5t #-O0 -fexpensive-optimizations -finline -ffast-math -msoft-float -falign-functions=32 -falign-loops -falign-labels -falign-jumps -fomit-frame-pointer 
CPP_MACRO = -D__SUPPORT_CPP_EXCEPTIONS__
EXE_MACRO = -D__EXE__
REL_MACRO = -DNDEBUG
DEB_MACRO = -D_DEBUG
UNICODE_MACRO = -D_UNICODE

GCCE_WARNING_FLAGS = -Wall -Wno-unknown-pragmas

CPP_ARGS = -undef -M -nostdinc -D__PRODUCT_INCLUDE__=\"$(OS_HRH)"
