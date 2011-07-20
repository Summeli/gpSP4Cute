PROJECT_NAME = gpsp4meego

USERINCLUDE = ../inc ../../gpsp4cute

CSRCS = \
		    gpspmain.c \
			cpu.c \
			video.c \
			input.c \
			sound.c \
			gui.c \
			zip.c \
      cpu_threaded.c \
			cheats.c \
			arm_stub_c.c \
			asm_debug.c \
			meego_adaptation.c \
			memory.c \
        $(NULL)
ASRCS = \
	    arm_stub.S \
	    video_blend.S \
	    relocator_glue.S \
		video_blend.S \
        $(NULL)
