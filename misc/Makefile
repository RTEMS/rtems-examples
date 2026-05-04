include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

SUBDIRS=minimum bspcmdline extract_example nanosecond_tick_wrap \
  qemu_vfat userdrv

# This has a surrogate "ada_main" in C so GNAT isn't needed.
#
# Note: There is not a way to know if RTEMS Ada support is included
#       so we do not build this by default.
# SUBDIRS += adamain

# If regulator.h is not present, do not build that example
ifneq (,$(RTEMS_MAKEFILE_PATH)/lib/include/rtems/regulator.h)
  SUBDIRS += regulator
endif

