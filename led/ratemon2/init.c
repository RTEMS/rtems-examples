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

#include <stdio.h>
#include <bsp.h>

#include "../../testmacros.h"
#include "../led.h"

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_id          period_id1;
  rtems_id          period_id2;
  rtems_interval    ticks;

  puts( "\n\n*** LED BLINKER -- two periods ***" );

  LED_INIT();

  status = rtems_rate_monotonic_create(
    rtems_build_name( 'P', 'E', 'R', '1' ),
    &period_id1
  );

  status = rtems_rate_monotonic_create(
    rtems_build_name( 'P', 'E', 'R', '2' ),
    &period_id2
  );

  ticks = get_ticks_per_second();

  status = rtems_rate_monotonic_period( period_id1, 2 * ticks );
  LED_ON();

  (void) rtems_task_wake_after( 1 * get_ticks_per_second() );
  status = rtems_rate_monotonic_period( period_id2, 2 * ticks );
  LED_OFF();

  while (1) {
    status = rtems_rate_monotonic_period( period_id1, 2 * ticks );
    LED_ON();

    status = rtems_rate_monotonic_period( period_id2, 2 * ticks );
    LED_OFF();
  }

  status = rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             4
#define CONFIGURE_MAXIMUM_PERIODS           2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
