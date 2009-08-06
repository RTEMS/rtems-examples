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
#include <stdint.h>
#include <stdio.h>

#include "../../testmacros.h"
#include "../led.h"

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
  rtems_event_set   events;
  uint32_t          count = 0;

  puts( "\n\n*** LED BLINKER -- event receive timeout ***" );

  LED_INIT();

  while (1) {

    status = rtems_event_receive( RTEMS_EVENT_1,
      RTEMS_DEFAULT_OPTIONS, get_ticks_per_second(), &events );
    if ( status != RTEMS_TIMEOUT )
      fputs( "receive did not timeout\n", stderr );

    if ( (count++ % 2) == 0 )
      LED_ON();
    else
      LED_OFF();

  }

  status = rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             4

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
