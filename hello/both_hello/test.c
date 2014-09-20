/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

long trace_test(int a, unsigned long b, long long c)
{
  printf ("int a=%i, unsigned long b=%u, long long c=%lli\n",
          a, b, c);
  return (long) (a + b);
}

rtems_task hello_init(
  rtems_task_argument ignored
)
{
  printf( "Classic -- Hello World\n" );
  //trace_test(1, 2, 3);
  rtems_task_delete( RTEMS_SELF );
}

#define CONFIGURE_INIT_TASK_ENTRY_POINT hello_init

int main(int argc, char** argv)
{
  hello_init(0);
}

void *POSIX_Init(
  void *argument
)
{
  printf( "POSIX -- Hello World\n" );
  sleep( 1 );
  //trace_test(1, 2, 3);
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
