/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */
/*  updated for triple test, 2003/11/06, Erik Adli */

#define CONFIGURE_INIT
#include "system.h"
#include <inttypes.h>
#include <stdio.h>

/*
 *  Keep the names and IDs in global variables so another task can use them.
 */
rtems_id   Task_id[ 4 ];         /* array of task ids */
rtems_name Task_name[ 4 ];       /* array of task names */

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_time_of_day time;
  uint32_t ticks_per_second, ticks_since_boot;

  puts( "\n\n\n*** PERIODIC TASKING TRIPLE TEST ***" );
  puts( "*** This demo shows three different ways of running periodic tasks ***" );
  puts( "*** It also demonstrates the CPU usage and Rate Monotonic statitistics utilities ***" );

  ticks_per_second = rtems_clock_get_ticks_per_second();
  printf("\nTicks per second in your system: %" PRIu32 "\n", ticks_per_second);
  ticks_since_boot = rtems_clock_get_ticks_since_boot();
  printf("Ticks since boot: %" PRIu32 "\n\n", ticks_since_boot);

  time.year   = 1988;
  time.month  = 12;
  time.day    = 31;
  time.hour   = 9;
  time.minute = 0;
  time.second = 0;
  time.ticks  = 0;

  status = rtems_clock_set( &time );

  Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );
  Task_name[ 2 ] = rtems_build_name( 'T', 'A', '2', ' ' );
  Task_name[ 3 ] = rtems_build_name( 'T', 'A', '3', ' ' );

  // prototype: rtems_task_create( name, initial_priority, stack_size, initial_modes, attribute_set, *id );
  status = rtems_task_create(
    Task_name[ 1 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 1 ]
  );
  status = rtems_task_create(
    Task_name[ 2 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 2 ]
  );
  status = rtems_task_create(
    Task_name[ 3 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 3 ]
  );

  // prototype: rtems_task_start( id, entry_point, argument );
  status = rtems_task_start( Task_id[ 1 ], Task_Absolute_Period, 1 );
  status = rtems_task_start( Task_id[ 2 ], Task_Rate_Monotonic_Period, 2 );
  status = rtems_task_start( Task_id[ 3 ], Task_Relative_Period, 3 );


  // delete init task after starting the three working tasks
  status = rtems_task_delete( RTEMS_SELF );
}
