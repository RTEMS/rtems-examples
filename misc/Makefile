include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

SUBDIRS=minimum bspcmdline extract_example nanosecond_tick_wrap userdrv

# If the POSIX API isn't enabled, we can't build these
ifeq ($(RTEMS_HAS_POSIX_API),yes)
# SUBDIRS += adamain
endif

# If regulator.h is not present, do not build that example
ifneq (,$(RTEMS_MAKEFILE_PATH).lib/include/rtems/regulator.h)
  SUBDIRS += regulator
endif

