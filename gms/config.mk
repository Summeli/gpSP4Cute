PROJECT_NAME = gpsp4symbian
TARGET_TYPE = LIB
UID2 = 0
UID3 = E0D278F3
SECUREID = E0D278F3
EPOCSTACKSIZE = 80000
EPOCHEAPSIZE = 5000000,67108864
CAPABILITY = LocalServices
SYSINCLUDE = $(EPOCROOT2)/include $(EPOCROOT2)/include/esdl $(EPOCROOT2)/include/libc
USERINCLUDE = ../inc ../../gpsp4cute
CXXSRCS = \
		symbian_memory_handler.cpp \
		sound_symbian.cpp \
		relocator.cpp \
         $(NULL)
CSRCS = \
		    gpspmain.c \
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
	    relocator_glue.S \
        $(NULL)
CERT = /c/cert/gpsp4symbian.cert
KEY = /c/cert/gpsp4symbian.key