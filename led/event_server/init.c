/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <bsp.h>
#include <inttypes.h>
#include <stdio.h>

#include "../led.h"

rtems_task Test_task(
  rtems_task_argument unused
)
{
  rtems_event_set events;

  for ( ; ; ) {
    events = 0;
    (void) rtems_event_receive(
      (RTEMS_EVENT_1 | RTEMS_EVENT_2),
      RTEMS_EVENT_ANY,
      RTEMS_NO_TIMEOUT,
      &events
    );

    if ( events == RTEMS_EVENT_1 ) {
      LED_OFF();
    } else if ( events == RTEMS_EVENT_2 ) {
      LED_ON();
    } else {
      fprintf( stderr, "Incorrect event set 0x%08" PRIx32 "\n", events );
    }
  }
}

rtems_task Init(
  rtems_task_argument argument
)
{
  uint32_t          count = 0;
  rtems_event_set   events;
  rtems_status_code status;
  rtems_id          task_id;
  rtems_name        task_name;

  puts( "\n\n*** LED BLINKER -- event receive server ***" );

  LED_INIT();

  task_name = rtems_build_name( 'T', 'A', '1', ' ' );

  (void) rtems_task_create(
    task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &task_id
  );

  (void) rtems_task_start( task_id, Test_task, 1 );

  for (count=0; ; count++) {

    events = ( (count % 2) == 0 ) ?  RTEMS_EVENT_1 : RTEMS_EVENT_2;
    status = rtems_event_send( task_id, events );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "send did not work\n", stderr );

    (void) rtems_task_wake_after( rtems_clock_get_ticks_per_second() );
  }

  (void) rtems_task_delete( RTEMS_SELF );
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
