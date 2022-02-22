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

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_interval    one_second;

  puts( "\n\n*** LED BLINKER -- task wake after ***" );

  one_second = 1 * rtems_clock_get_ticks_per_second();

  LED_INIT();

  while (1) {

    (void) rtems_task_wake_after( one_second );
    LED_OFF();
    (void) rtems_task_wake_after( one_second );
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
