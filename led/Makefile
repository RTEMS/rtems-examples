include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

SUBDIRS=delay event_server msg_server sem_server ratemon1 ratemon2 \
  timeout_event timer timer_server

# If the POSIX API isn't enabled, we can't build these
ifeq ($(RTEMS_HAS_POSIX_API),yes)
SUBDIRS += posix_cond_server posix_delay_sleep posix_delay_nanosleep \
  posix_mutex_server ratemon_cond_server
  # This test requires POSIX and C++
  ifneq ($(CXX_FOR_TARGET),)
    SUBDIRS += complex1
  endif
endif
