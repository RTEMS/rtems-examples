#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

VPATH=.:nbench-byte-2.2.3
PGM=${ARCH}/nbench.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = main.c \
emfloat.c \
hardware.c \
misc.c \
nbench0.c \
nbench1.c \
sysspec.c

COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

LINK_LIBS += -lm
OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
