#
#  Example Library Makefile
#

#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

LIB=${ARCH}/libUserLib.a

# C source names
CSRCS = file_1.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

EXTERNAL_H_FILES=userLib.h

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

CLEAN_ADDITIONS += $(LIB)

OBJS= $(COBJS)

all:    ${ARCH} $(LIB)

$(LIB): $(OBJS)
	$(AR) crv $(LIB) $(OBJS)

install: all
	$(INSTALL_VARIANT) -m 555 ${LIB} ${PROJECT_RELEASE}/lib
	$(INSTALL_VARIANT) -m 555 ${EXTERNAL_H_FILES} ${PROJECT_RELEASE}/include
