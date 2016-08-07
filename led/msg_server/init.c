/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
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

  for ( ; ; ) {
    count = 0xFFFFFFFF;
    (void) rtems_message_queue_receive(
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
  uint32_t          count;
  rtems_id          task_id;
  rtems_status_code status;

  puts( "\n\n*** LED BLINKER -- message receive server ***" );

  LED_INIT();

  (void) rtems_message_queue_create(
    rtems_build_name( 'Q', '1', ' ', ' ' ),
    1,
    sizeof(uint32_t),
    RTEMS_DEFAULT_ATTRIBUTES,
    &Queue_id
  );

  (void) rtems_task_create(
    rtems_build_name( 'T', 'A', '1', ' ' ),
    1,
    RTEMS_MINIMUM_STACK_SIZE,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &task_id
  );

  (void) rtems_task_start( task_id, Test_task, 1 );

  for (count=0; ; count++) {

    /* fprintf( stderr, "send 0x%d\n", count ); */
    status = rtems_message_queue_send( Queue_id, &count, sizeof(uint32_t) );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "send did not work\n", stderr );

    (void) rtems_task_wake_after( rtems_clock_get_ticks_per_second() );
  }

  (void) rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             2
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES    1
#define CONFIGURE_MESSAGE_BUFFER_MEMORY \
           CONFIGURE_MESSAGE_BUFFERS_FOR_QUEUE(1, sizeof(uint32_t))

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
