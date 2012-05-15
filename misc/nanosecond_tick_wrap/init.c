/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

/* Based upon code in PR1748 */

#include <stdio.h>
#include <stdlib.h>

#include <bsp.h>

#define LOOPS 1000
uint32_t           uptime_usec[LOOPS];
struct timespec    uptime[LOOPS];

rtems_task Init(
  rtems_task_argument argument
)
{
  int              i;
  uint32_t         t0=0;

  puts( "\n\n*** Nanosecond Clock Tick Wrap Test ***" );
  for ( i=0; i<LOOPS; i++ ) {

    rtems_clock_get_uptime( &uptime[i] );
    uptime_usec[i] = ((uint32_t)uptime[i].tv_sec * 1000000) +
                     ((uint32_t)uptime[i].tv_nsec / 1000);
  }

  for ( i=0; i<LOOPS; i++ ) {
    unsigned t1, usec, msec, sec;

    t1 = uptime_usec[i];
    usec = t1 % 1000;
    msec = (t1 / 1000) % 1000;
    sec  = t1 / 1000000;

    printf(
      "TEST:rtems=%u.%09u   t=%u.%03u.%03u   dt=%u\n",
      uptime[i].tv_sec, uptime[i].tv_nsec, sec, msec, usec, (t1-t0)
    );
    t0 = t1;
  }

  puts( "*** END OF Nanosecond Clock Tick Wrap Test ***" );
  exit(0);
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS         1
#define CONFIGURE_MICROSECONDS_PER_TICK \
        (RTEMS_MILLISECONDS_TO_MICROSECONDS(1) / 2)

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/

