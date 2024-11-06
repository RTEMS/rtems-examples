/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * This file contains the RTEMS Configuration for this example.
 * It provides a POSIX Initialization thread which performs initialization
 * of support capabilities (e.g. networking, mounting filesystems, etc.)
 * and then invokes main(). By following the pattern of having RTEMS
 * configuration and initialization contained in a separate file, the
 * application can be portable POSIX code with no RTEMS references.
 */

/*
 * Copyright 2024 Joel Sherrill (joel@rtems.org)
 *
 * This file's license is 2-clause BSD as in this distribution's LICENSE.2 file.
 */

#include <stdlib.h>

int main(int argc, char **argv);

static char *argv_list[] = {
  "hello",
  ""
};

static void *POSIX_Init(void *arg)
{
  (void) arg;  /* deliberately ignored */

  int rc;

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


/* configuration information */

#include <bsp.h>

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_MAXIMUM_POSIX_THREADS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
