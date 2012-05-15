include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

# If the POSIX API isn't enabled, we can't build these
ifeq ($(RTEMS_HAS_POSIX_API),yes)
  SUBDIRS = psx_example_1 psx_example_2 psx_example_3 psx_sched_report
endif
