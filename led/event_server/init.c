/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <bsp.h>
#include <inttypes.h>
#include <stdio.h>

#include "../../testmacros.h"
#include "../led.h"

rtems_task Test_task(
  rtems_task_argument unused
)
{
  rtems_event_set   events;
  rtems_status_code status;

  LED_INIT();

  for ( ; ; ) {
    events = 0;
    status = rtems_event_receive(
      (RTEMS_EVENT_1 | RTEMS_EVENT_2),
      RTEMS_EVENT_ANY,
      RTEMS_NO_TIMEOUT,
      &events
    );

    if ( events == RTEMS_EVENT_1 ) {
      LED_ON();
    } else if ( events == RTEMS_EVENT_2 ) {
      LED_OFF();
    } else {
      fprintf( stderr, "Incorrect event set 0x%08" PRIx32 "\n", events );
    }
  }
}

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
  uint32_t          count = 0;
  rtems_event_set   events;

  puts( "\n\n*** LED BLINKER -- event receive server ***" );

  Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );

  status = rtems_task_create(
    Task_name[ 1 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 1 ]
  );

  status = rtems_task_start( Task_id[ 1 ], Test_task, 1 );

  while (1) {

    events = ( (count++ % 2) == 0 ) ?  RTEMS_EVENT_1 : RTEMS_EVENT_2;
    status = rtems_event_send( Task_id[ 1 ], events );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "send did not work\n", stderr );

    status = rtems_task_wake_after( get_ticks_per_second() );
  }

  status = rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
