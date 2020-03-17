/*
 *  Starting point for this example.  Can be compiled on RTEMS or Linux.
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

extern void doTest(void);

#if defined(__rtems__)
#include <bsp.h>

extern "C" {
  rtems_task Init(
    rtems_task_argument arg
  );
};

rtems_task Init(
  rtems_task_argument arg
)
#else
int main(
  int    argc,
  char **argv
)
#endif
{
#if !defined(BSP_SMALL_MEMORY)
  doTest();
#endif
  exit( 0 );
}

#if defined(__rtems__)
/* RTEMS configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MICROSECONDS_PER_TICK RTEMS_MILLISECONDS_TO_MICROSECONDS(1)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#if defined(BSP_SMALL_MEMORY)
  #define CONFIGURE_MAXIMUM_TASKS 1
#else
  #define CONFIGURE_MAXIMUM_TASKS 102
  #define CONFIGURE_MAXIMUM_PERIODS  100
  #define CONFIGURE_MAXIMUM_POSIX_THREADS 2
  #define CONFIGURE_EXTRA_TASK_STACKS (32 * 1024)

  #define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
  #define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 20

  #define CONFIGURE_STACK_CHECKER_ENABLED
#endif

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
#endif

/* end of file */
