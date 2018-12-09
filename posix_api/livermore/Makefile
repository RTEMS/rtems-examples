include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

# If the POSIX API isn't enabled, we can't build these
ifeq ($(RTEMS_HAS_POSIX_API),yes)
  SUBDIRS  = pthread
endif
