/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <stdio.h>
#include <bsp.h>

#include "../led.h"

rtems_id   Queue_id;

rtems_task Test_task(
  rtems_task_argument unused
)
{
  uint32_t           count;
  size_t             received;
  rtems_status_code  status;

  for ( ; ; ) {
    count = 0xFFFFFFFF;
    status = rtems_message_queue_receive(
      Queue_id,
      (void *) &count,
      &received,
      RTEMS_DEFAULT_OPTIONS,
      RTEMS_NO_TIMEOUT
    );

    if ( (count % 2) == 0 ) {
      LED_OFF();
    } else {
      LED_ON();
    }
  }
}

rtems_task Init(
  rtems_task_argument argument
)
{
  uint32_t          count = 0;
  rtems_status_code status;
  rtems_id          task_id;
  rtems_name        task_name;

  puts( "\n\n*** LED BLINKER -- message receive server ***" );

  LED_INIT();

  status = rtems_message_queue_create(
    rtems_build_name( 'Q', '1', ' ', ' ' ),
    1,
    sizeof(uint32_t),
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id
  );

  task_name = rtems_build_name( 'T', 'A', '1', ' ' );

  status = rtems_task_create(
    task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &task_id
  );

  status = rtems_task_start( task_id, Test_task, 1 );

  for (count=0; ; count++) {

    /* fprintf( stderr, "send 0x%d\n", count ); */
    status = rtems_message_queue_send( Queue_id, &count, sizeof(uint32_t) );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "send did not work\n", stderr );

    status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() );
  }

  status = rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             2
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES    1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
