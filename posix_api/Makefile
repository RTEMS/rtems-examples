include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(RTEMS_SHARE)/make/directory.cfg

# These use POSIX
SUBDIRS  = psx_pthread_join
SUBDIRS += psx_pthread_join_condvar
SUBDIRS += psx_pthread_priority
SUBDIRS += psx_barrier_report
SUBDIRS += psx_condvar_report
SUBDIRS += psx_mqueue_report
SUBDIRS += psx_mutex_report
SUBDIRS += psx_pthread_report
SUBDIRS += psx_rwlock_report
SUBDIRS += psx_sched_report
SUBDIRS += psx_task_restart
SUBDIRS += psx_thread_restart
SUBDIRS += livermore

# If the POSIX API isn't enabled, we can't build these
ifeq ($(RTEMS_HAS_POSIX_API),yes)
  SUBDIRS += psx_sigint
endif
