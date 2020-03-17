/**
 *  @brief RTEMS Configuration for C11 Mutex Example
 */

/*
 * This file's license is 2-clause BSD as in this distribution's
 * LICENSE.2 file.
 */

#include <stdlib.h>

int main(int argc, char **argv);

static char *argv_list[] = {
  "c11-mutex01",
  ""
};

static void *POSIX_Init(void *arg)
{
  int rc;

  (void) arg;  /* deliberately ignored */

  /*
   * Initialize optional services
   */

  /*
   * Could get arguments from command line or have a static set.
   */
  rc = main(1, argv_list);

  exit(rc);
  return NULL;
}

#include <bsp.h> /* for device driver prototypes */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 32

#define CONFIGURE_MINIMUM_TASK_STACK_SIZE (64 * 1024)

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
