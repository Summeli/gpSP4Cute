#
#   Copyright 2009, Harry Li <harry.li AT pagefreedom.org>
#             2009, Antti Pojola < summeli AT summeli.fi>
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

EPOCROOT2 = $(SYMBIAN_SDK_PATH)Epoc32
#path could be easily extended with /c/Program\ Files/CodeSourcery/SourceGLite/bin:
PATH := $(EPOCROOT2)/tools:$(EPOCROOT2)/gcc/bin:$(PATH)

# Defaults
PKG_PATH = ../sis/$(PROJECT_NAME)

SRC_PATH = ../src
RSS_PATH = ../data
OBJ_BASE = obj
DIST_BASE = dist
VENDORID = 0

include config.mk
include defines.mk

check:
	@echo 'SYMBIAN_SDK_PATH is' $(SYMBIAN_SDK_PATH)
	@echo 'ACTIVE_PERL is ' $(ACTIVE_PERL)
	@echo 'CC_INSTALL_PATH is ' $(CC_INSTALL_PATH)
	@echo 'PATH is ' $(PATH)
	@echo '==============================================================='

preGCCE:
  CC = arm-none-symbianelf-gcc
  CXX = arm-none-symbianelf-g++
  LD = arm-none-symbianelf-ld
  AS = arm-none-symbianelf-as.exe
  RAL = $(EPOCROOT2)/RELEASE/ARMV5/LIB
  DIST_PATH = $(DIST_BASE)/gcce
  OBJ_PATH = $(OBJ_BASE)/gcce
  TARGET_MACRO = $(GCCE_MACRO) $(GCCE_WARNING_FLAGS)

preDEBUG:
   RAU = $(EPOCROOT2)/RELEASE/ARMV5/UDEB
  COMPFLAGS = -march=armv5t -O0 -mapcs -pipe -nostdinc -c -msoft-float -g \
	-MD $(DEB_MACRO) -include $(EPOCROOT2)/include/gcce/gcce.h
  LD_EXE_FLAGS = -L$(CC_INSTALL_PATH)/arm-none-symbianelf/lib \
  -L$(EPOCROOT2)/RELEASE/ARMV5/UDEB  \
	-L$(CC_INSTALL_PATH)/lib/gcc/arm-none-symbianelf/3.4.3 --target1-abs \
	--no-undefined -nostdlib -shared -Ttext 0x8000 -Tdata 0x400000 \
	--default-symver -soname $(PROJECT_NAME){000a0000}[$(UID3)].exe \
	--entry _E32Startup -u _E32Startup \
	$(EPOCROOT2)/RELEASE/ARMV5/UDEB/EEXE.LIB \
	-o $(OBJ_PATH)/$(PROJECT_NAME).exe \
	-Map $(BIN_TARGET).map $^ \
	$(patsubst %.lib,$(RAL)/%.dso,$(SYSLIBRARY)) \
	$(RAU)/usrt2_2.lib $(RAL)/dfpaeabi.dso $(RAL)/dfprvct2_2.dso \
	$(RAL)/drtaeabi.dso $(RAL)/scppnwdl.dso $(RAL)/drtrvct2_2.dso \
	$(patsubst %.lib,$(RAU)/%.lib,$(STATICLIBRARY)) \
	-lsupc++ -lgcc
	COPYSYMBOLS = cp $(OBJ_PATH)/$(PROJECT_NAME).$(TARGET_TYPE) $(DIST_PATH)/$(PROJECT_NAME).sym
  COPYEXEC = cp $(DIST_PATH)/$(PROJECT_NAME).$(TARGET_TYPE) $(EPOCROOT2)/release/GCCE/UDEB/$(PROJECT_NAME).$(TARGET_TYPE)
  COPYSYMB = cp $(DIST_PATH)/$(PROJECT_NAME).sym $(EPOCROOT2)/release/GCCE/UDEB/$(PROJECT_NAME).sym
  COPYMAP = cp $(DIST_PATH)/$(PROJECT_NAME).$(TARGET_TYPE).map $(EPOCROOT2)/release/GCCE/UDEB/$(PROJECT_NAME).$(TARGET_TYPE).map
	
preREL:
   RAU = $(EPOCROOT2)/RELEASE/ARMV5/UREL
   COMPFLAGS = -march=armv5t -O2 -mapcs -pipe -nostdinc -c -msoft-float \
	-MD $(REL_MACRO) -include $(EPOCROOT2)/include/gcce/gcce.h
  LD_EXE_FLAGS = -L$(CC_INSTALL_PATH)/arm-none-symbianelf/lib \
  -L$(EPOCROOT2)/RELEASE/ARMV5/UREL  \
	-L$(CC_INSTALL_PATH)/lib/gcc/arm-none-symbianelf/3.4.3 --target1-abs \
	--no-undefined -nostdlib -shared -Ttext 0x8000 -Tdata 0x400000 \
	--default-symver -soname $(PROJECT_NAME){000a0000}[$(UID3)].exe \
	--entry _E32Startup -u _E32Startup \
	$(EPOCROOT2)/RELEASE/ARMV5/UREL/EEXE.LIB \
	-o $(OBJ_PATH)/$(PROJECT_NAME).exe \
	-Map $(BIN_TARGET).map $^ \
	$(patsubst %.lib,$(RAL)/%.dso,$(SYSLIBRARY)) \
	$(RAU)/usrt2_2.lib $(RAL)/dfpaeabi.dso $(RAL)/dfprvct2_2.dso \
	$(RAL)/drtaeabi.dso $(RAL)/scppnwdl.dso $(RAL)/drtrvct2_2.dso \
	$(patsubst %.lib,$(RAU)/%.lib,$(STATICLIBRARY)) \
	-lsupc++ -lgcc
	COPYSYMBOLS = cp $(OBJ_PATH)/$(PROJECT_NAME).$(TARGET_TYPE) $(DIST_PATH)/$(PROJECT_NAME).sym
  COPYEXEC = cp $(DIST_PATH)/$(PROJECT_NAME).$(TARGET_TYPE) $(EPOCROOT2)/release/GCCE/UREL/$(PROJECT_NAME).$(TARGET_TYPE)
  COPYSYMB = cp $(DIST_PATH)/$(PROJECT_NAME).sym $(EPOCROOT2)/release/GCCE/UREL/$(PROJECT_NAME).sym
  COPYMAP = cp $(DIST_PATH)/$(PROJECT_NAME).$(TARGET_TYPE).map $(EPOCROOT2)/release/GCCE/UREL/$(PROJECT_NAME).$(TARGET_TYPE).map
	
preALL:
  CFLAGS += $(COMPFLAGS) $(UNICODE_MACRO) $(TARGET_MACRO) $(SYMBIAN_MACRO) $(S60_MACRO) \
	$(EXE_MACRO) $(addprefix -D ,$(PRJ_MACRO)) \
	-D__PRODUCT_INCLUDE__=$(OS_HRH) -I $(SRC_PATH) -I $(VARIANT) \
	$(addprefix -I ,$(USERINCLUDE)) $(addprefix -I ,$(SYSINCLUDE)) \
	-I $(CC_INSTALL_PATH)/lib/gcc/arm-none-symbianelf/3.4.3/include
  CXXFLAGS = $(CFLAGS) -Wno-ctor-dtor-privacy -x c++ $(CPP_MACRO)
  C_OBJS = $(patsubst %.c,$(OBJ_PATH)/%.o,$(CSRCS))
  CXX_OBJS = $(patsubst %.cpp,$(OBJ_PATH)/%.o,$(CXXSRCS))
  A_OBJS =  $(patsubst %.S,$(OBJ_PATH)/%.o,$(ASRCS))
  BIN_TARGET = $(DIST_PATH)/$(PROJECT_NAME).$(TARGET_TYPE)
  ASFLAGS = -mfloat-abi=soft
  
$(IMG_TARGETS): $(IMG_SRCS)
	$(EPOCROOT2)/tools/mifconv $@ $(subst /,//,$(IMG_ARGS)) $^

$(RSS_TARGETS): $(DIST_PATH)/%.rsc : $(RSS_PATH)/%.rss
	$(ACTIVE_PERL) -S $(EPOCROOT2)/tools/epocrc.pl -m045,046,047 \
	$(addprefix -I ,$(RSS_PATH)) $(addprefix -I ,$(USERINCLUDE)) -I- \
	$(addprefix -I ,$(SYSINCLUDE)) $(addprefix -D ,$(LANG_MACRO)) \
	-I $(VARIANT) -t$(OBJ_PATH) -u -v $< -o$@ \
	-h$(EPOCROOT2)/include/$(notdir $(basename $@)).rsg

$(CXX_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(C_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(A_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.S
	$(CC) -c -o $@ $^
	  	
$(BIN_TARGET): $(C_OBJS) $(CXX_OBJS) $(A_OBJS)
	$(LD) $(LD_EXE_FLAGS)
	$(COPYSYMBOLS)
	$(EPOCROOT2)/tools/elf2e32 --sid=0x$(SECUREID) --stack=$(EPOCSTACKSIZE) --heap=$(EPOCHEAPSIZE) \
	--vid=0x$(VENDORID) --uid1=0x1000007a --uid2=0x$(UID2) --uid3=0x$(UID3) \
	 --capability=$(CAPABILITY) --dlldata --defoutput=$(PROJECT_NAME).def \
	 --fpu=softvfp --targettype=$(TARGET_TYPE) \
	 --output=$@ --elfinput=$(OBJ_PATH)/$(PROJECT_NAME).$(TARGET_TYPE) \
	 --linkas=$(PROJECT_NAME){000a0000}[$(UID3)].$(TARGET_TYPE) \
	 --libpath=$(RAL)
	 $(COPYEXEC)
	 $(COPYSYMB)
	 $(COPYMAP)
	  
pack: buildg $(PKG_PATH).pkg
	$(EPOCROOT2)/tools/makesis.exe -v $(PKG_PATH).pkg $(PKG_PATH).sis
	$(EPOCROOT2)/tools/signsis.exe -v $(PKG_PATH).sis $(PKG_PATH).sisx \
	$(CERT) $(KEY) $(PASS)

cleang: check preGCCE preALL
	rm -rf $(IMG_TARGETS)
	rm -rf $(RSS_TARGETS)
	rm -rf $(C_OBJS) $(CXX_OBJS) $(A_OBJS)
	rm -rf $(BIN_TARGET)

cleanw: check

buildw: check

clean: cleang

debug: check preGCCE preDEBUG preALL $(IMG_TARGETS) $(RSS_TARGETS) $(BIN_TARGET)

release: check preGCCE preREL preALL $(IMG_TARGETS) $(RSS_TARGETS) $(BIN_TARGET)
