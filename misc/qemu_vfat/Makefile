#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/qemu_vfat.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = init.c start_test.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

# Define this if you want to run the shell after mounting
# DEFINES += -DUSE_SHELL
# Defint this if you want to call start_test() instead
DEFINES += -DUSE_START_TEST

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
