#
#   Copyright 2011, Antti Pojola < summeli AT summeli.fi>
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

MADDE_PATH = /e/QtSDK/Madde
MADDE_GCC_INSTALL_PATH =$(MADDE_PATH)/toolchains/arm-2009q3-67-arm-none-linux-gnueabi-i586-mingw32msvc/arm-2009q3-67/bin
MADDE_INCLUDE = $(MADDE_PATH)/sysroots/harmattan-nokia-meego-arm-sysroot-1122-slim/usr/include
PATH := $(MADDE_GCC_INSTALL_PATH):$(PATH)

SYSINCLUDE = $(MADDE_PATH)/sysroots/harmattan-nokia-meego-arm-sysroot-1122-slim/usr/lib/gcc/arm-linux-gnueabi/4.4/include \
             $(MADDE_INCLUDE)

#Macros for compilation process
GCC_MACRO = -D__GCCE__ -D__MARM__ -D__MARM_ARMV5__ -D__EABI__
CPP_MACRO = -D__SUPPORT_CPP_EXCEPTIONS__
EXE_MACRO = -D__EXE__
REL_MACRO = -DNDEBUG
DEB_MACRO = -D_DEBUG
UNICODE_MACRO = -D_UNICODE
GCC_WARNING_FLAGS = -Wall -Wno-unknown-pragmas
CPP_ARGS = -undef -M -nostdinc

# paths
SRC_PATH = ../src
OBJ_BASE = obj
DIST_BASE = dist

include config.mk
check:
	@echo 'MADDE_PATH is' $(MADDE_PATH)
	@echo 'MADDE_INCLUDE is ' $(MADDE_INCLUDE)
	@echo 'MADDE_GCC_INSTALL_PATH is ' $(MADDE_GCC_INSTALL_PATH)
#	@echo 'PATH is ' $(PATH)
#	@echo '==============================================================='

preGCC:
  CC = arm-none-linux-gnueabi-gcc
  CXX = arm-none-linux-gnueabi-g++
  LD = arm-none-linux-gnueabi-ld
  AS = arm-none-linux-gnueabi-as
  AR = arm-none-linux-gnueabi-ar
  DIST_PATH = $(DIST_BASE)/gcce
  OBJ_PATH = $(OBJ_BASE)/gcce
  TARGET_MACRO = $(GCC_MACRO) $(GCC_WARNING_FLAGS)

preDEBUG:
   COMPFLAGS = -march=armv7-a -O0 -mapcs -pipe -nostdinc -c -g \
   -MD $(DEB_MACRO)
   
#preREL:
#   COMPFLAGS = -march=armv7-a -O2 -mapcs -pipe -nostdinc -c \
#   -MD $(REL_MACRO)

preALL:
  CFLAGS += $(COMPFLAGS) $(UNICODE_MACRO) $(TARGET_MACRO) \
	$(EXE_MACRO) $(addprefix -D ,$(PRJ_MACRO)) \
  -I $(SRC_PATH) $(addprefix -I ,$(USERINCLUDE)) $(addprefix -I ,$(SYSINCLUDE))
  CXXFLAGS = $(CFLAGS) -Wno-ctor-dtor-privacy -x c++ $(CPP_MACRO)
  C_OBJS = $(patsubst %.c,$(OBJ_PATH)/%.o,$(CSRCS))
  CXX_OBJS = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(CXXSRCS))
  A_OBJS =  $(patsubst %.S,$(OBJ_PATH)/%.o,$(ASRCS))
  BIN_TARGET = $(DIST_PATH)/$(PROJECT_NAME).a
  ASFLAGS = -mfloat-abi=soft
  COPYLIB = cp $(DIST_PATH)/$(PROJECT_NAME).a ../$(PROJECT_NAME).a

$(CXX_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(C_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(A_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.S
	$(CC) -c -o $@ $^
	  	
$(BIN_TARGET): $(C_OBJS) $(CXX_OBJS) $(A_OBJS)
	$(AR) rsc $(BIN_TARGET) $(C_OBJS) $(CXX_OBJS) $(A_OBJS)
	$(COPYLIB)

cleang: check preGCC preALL
	rm -rf $(C_OBJS) $(CXX_OBJS) $(A_OBJS)
	rm -rf $(BIN_TARGET)

cleanw: check

buildw: check

clean: cleang

debug: check preGCC preDEBUG preALL $(BIN_TARGET)

release: check preGCC preREL preALL $(BIN_TARGET)
