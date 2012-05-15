#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/display_cpu.exe

# optional managers required
MANAGERS=all

# C source names
SRCS = test.c
OBJS = $(SRCS:%.[cS]=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)
