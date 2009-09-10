# ============================================================================
#  Name	 : build_help.mk
#  Part of  : gpS60P
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : gpS60P
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : gpS60P.hlp
gpS60P.hlp : gpS60P.xml gpS60P.cshlp Custom.xml
	cshlpcmp gpS60P.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy gpS60P.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del gpS60P.hlp
	del gpS60P.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo gpS60P.hlp

FINAL : do_nothing
