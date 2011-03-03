/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  printf( "Classic -- Hello World\n" );
  rtems_task_delete( RTEMS_SELF );
}

void *POSIX_Init(
  void *argument
)
{
  printf( "POSIX -- Hello World\n" );
  sleep( 1 );
  exit( 0 );
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_TASKS 1
#define CONFIGURE_MAXIMUM_POSIX_THREADS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
