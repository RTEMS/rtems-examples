/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <bsp.h>
#include <stdint.h>
#include <stdio.h>

#include "../led.h"

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_event_set   events;
  uint32_t          count = 0;

  puts( "\n\n*** LED BLINKER -- event receive timeout ***" );

  LED_INIT();

  for (count=0; ; count++) {

    status = rtems_event_receive(
      RTEMS_EVENT_1,
      RTEMS_DEFAULT_OPTIONS,
      rtems_clock_get_ticks_per_second(),
      &events
    );
    if ( status != RTEMS_TIMEOUT )
      fputs( "receive did not timeout\n", stderr );

    if ( (count % 2) == 0 )
      LED_OFF();
    else
      LED_ON();

  }

  (void) rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
