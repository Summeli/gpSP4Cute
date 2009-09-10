PROJECT_NAME = gpSP4Symbian
TARGET_TYPE = EXE
UID2 = 0
UID3 = E0D278F3
SECUREID = E0D278F3
EPOCSTACKSIZE = 80000
EPOCHEAPSIZE = 5000000,67108864
CAPABILITY = LocalServices
SYSINCLUDE = $(EPOCROOT2)/include $(EPOCROOT2)/include/esdl $(EPOCROOT2)/include/libc
USERINCLUDE = ../inc
CXXSRCS = \
		gpsp4symbian_sdl.cpp \
		debug.cpp \
		symbian_memory_handler.cpp \
		sound_symbian.cpp \
         $(NULL)
CSRCS = \
		    main.c \
			cpu.c \
			video.c \
			sound.c \
			input.c \
			cpu_threaded.c \
			gui.c \
			zip.c \
			cheats.c \
			memory.c \
			arm_stub_c.c \
			asm_debug.c \
        $(NULL)
ASRCS = \
	    arm_stub.S \
	    video_blend.S \
        $(NULL)
RSS_TARGETS = $(DIST_PATH)/$(PROJECT_NAME).rsc $(DIST_PATH)/$(PROJECT_NAME)_reg.rsc
LANG_MACRO = SC
SYSLIBRARY = euser.lib avkon.lib eikcore.lib eiksrv.lib apparc.lib estlib.lib efsrv.lib cone.lib hal.lib \
			fbscli.lib ws32.lib  apgrfx.lib gdi.lib bitgdi.lib  mediaclientaudiostream.lib bafl.lib estor.lib \
			commonengine.lib hlplch.lib ezlib.lib libc.lib flogger.lib form.lib tagma.lib\
             $(NULL)
STATICLIBRARY = ESDL.lib
CERT = /c/cert/gpsp4symbian.cert
KEY = /c/cert/gpsp4symbian.key