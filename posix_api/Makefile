include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

# If the POSIX API isn't enabled, we can't build these
ifeq ($(RTEMS_HAS_POSIX_API),yes)
  SUBDIRS  = psx_example_1
  SUBDIRS += psx_example_2
  SUBDIRS += psx_example_3
  SUBDIRS += psx_barrier_report
  SUBDIRS += psx_condvar_report
  SUBDIRS += psx_mqueue_report
  SUBDIRS += psx_mutex_report
  SUBDIRS += psx_pthread_report
  SUBDIRS += psx_rwlock_report
  SUBDIRS += psx_sched_report
  SUBDIRS += livermore
endif
