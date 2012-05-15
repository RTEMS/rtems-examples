# Templates/Makefile.lib
#       adapted for an external library
#

LIBNAME=libfoo.a        # xxx- your library names goes here
LIB=${ARCH}/${LIBNAME}

# C and C++ source names, if any, go here -- minus the .c or .cc
C_FILES=
C_O_FILES=$(C_FILES:%.c=${ARCH}/%.o)

CC_FILES=foo.cc
CC_O_FILES=$(CC_FILES:%.cc=${ARCH}/%.o)

CPP_FILES=foo1.cpp
CPP_O_FILES=$(CPP_FILES:%.cpp=${ARCH}/%.o)

H_FILES=foo.h

SRCS=$(C_FILES) $(CC_FILES) $(CPP_FILES) $(H_FILES)
OBJS=$(C_O_FILES) $(CC_O_FILES) $(CPP_O_FILES)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/lib.cfg

#
# Extra rules for other variations on C++ file name extensions
#

COMPILE.cpp=$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c

${ARCH}/%.o: %.cpp
	${COMPILE.cc} -o $@ $<

#
# Add local stuff here using +=
#

DEFINES  +=
CPPFLAGS +=
CFLAGS   +=

#
# Add your list of files to delete here.  The config files
#  already know how to delete some stuff, so you may want
#  to just run 'make clean' first to see what gets missed.
#  'make clobber' already includes 'make clean'
#
CLEAN_ADDITIONS +=
CLOBBER_ADDITIONS +=

all:	${ARCH} $(SRCS) $(LIB)

$(LIB): ${OBJS}
	$(make-library)

# Install the library, appending _g or _p as appropriate.
# for include files, just use $(INSTALL)
install:  all
	# $(INSTALL_VARIANT) -m 644 ${LIB} ${PROJECT_RELEASE}/lib
