#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/complex1.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = periodic.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

# C++ source names
CXXSRCS = main.cc menu.cc test.cc
CXXOBJS_ = $(CXXSRCS:.cc=.o)
CXXOBJS = $(CXXOBJS_:%=${ARCH}/%)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
LINK.c=$(CXX)
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-cxx-exe)
