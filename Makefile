#
#  $Id$
#

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_ROOT)/make/directory.cfg

SUBDIRS=classic_api posix_api hello led misc ticker file_io

# Monitor Specific Examples
SUBDIRS += uboot

# Language Specific Examples
SUBDIRS += cxx
