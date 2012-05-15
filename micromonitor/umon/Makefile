#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

VPATH=.
PGM=${ARCH}/umon.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = test.c ## monlib.c tfsDriver.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

## DEFINES += -I.

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
