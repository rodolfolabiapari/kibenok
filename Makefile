NAME=kibenok
LOCAL_CONFIG = -legup-config=config.tcl
LEVEL = ..
include $(LEVEL)/Makefile.common



# Added from Rodolfo

# Linker true
LINK = 1

# Clang debug
DEBUG_G_FLAG = 1

# Perform in-system
DEBUGGER = 1

#CFLAG       += `pkg-config --libs --cflags opencv`
#CLANG_FLAGS += `pkg-config --libs --cflags opencv`
#OPT_FLAGS   += `pkg-config --libs opencv`
