/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>

void print_sched_info(
  char *s,
  int policy
)
{
  int min, max, levels;
  struct timespec t;

  printf( "Information on %s\n", s );
  min = sched_get_priority_min( policy );
  max = sched_get_priority_max( policy );
  (void) sched_rr_get_interval( 1, &t );
  levels = max - min;
  printf( "\tSupports %d priority levels (%d - %d)\n", levels, min, max  );
  if ( levels >= 32 )
    printf( "\tImplementation is compliant on priority levels\n");
  else
    printf( "\tImplementation is NOT compliant on priority levels\n" );

  printf( "\tScheduling quantum is %ld seconds and %ld nanoseconds\n",
              (long)t.tv_sec, (long)t.tv_nsec );
}


#if defined(__rtems__)
  #include <bsp.h>

rtems_task Init(
  rtems_task_argument ignored
)
#else
int main()
#endif
{
  print_sched_info( "SCHED_OTHER", SCHED_OTHER );
  print_sched_info( "SCHED_FIFO", SCHED_FIFO );
  print_sched_info( "SCHED_RR", SCHED_RR );

  exit( 0 );
}

#if defined(__rtems__)
/* configuration information */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MICROSECONDS_PER_TICK 1000
#define CONFIGURE_TICKS_PER_TIMESLICE   1
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
#endif

/* end of file */
