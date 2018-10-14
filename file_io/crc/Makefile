#
#  RTEMS_MAKEFILE_PATH is typically set in an environment variable
#

PGM=${ARCH}/crc.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = crc_32.c init.c fs-root-tar.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

CLEAN_ADDITIONS += stamp-fs-image
CLEAN_ADDITIONS += fs-root-tar fs-root-tar.c fs-root-tar.h

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(OBJS) $(PGM)

$(ARCH)/init.o: init.c stamp-fs-image

$(ARCH)/fs-root-tar.o: stamp-fs-image fs-root-tar.c fs-root-tar.h

fs-root-tar: $(ARCH) rootfs/image.img
	cd rootfs ; \
	    tar cf ../fs-root-tar --exclude CVS --exclude .cvsignore .

stamp-fs-image: $(ARCH) fs-root-tar
	rtems-bin2c fs-root-tar fs-root-tar
	ls -l fs-root-tar*
	touch stamp-fs-image

$(PGM): $(OBJS)
	$(make-exe)
