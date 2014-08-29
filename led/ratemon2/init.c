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
  rtems_id          period_id1;
  rtems_id          period_id2;
  rtems_interval    ticks;

  puts( "\n\n*** LED BLINKER -- two periods ***" );

  LED_INIT();

  (void) rtems_rate_monotonic_create(
    rtems_build_name( 'P', 'E', 'R', '1' ),
    &period_id1
  );

  (void) rtems_rate_monotonic_create(
    rtems_build_name( 'P', 'E', 'R', '2' ),
    &period_id2
  );

  ticks = rtems_clock_get_ticks_per_second();

  (void) rtems_rate_monotonic_period( period_id1, 2 * ticks );
  LED_OFF();

  (void) rtems_task_wake_after( ticks );
  (void) rtems_rate_monotonic_period( period_id2, 2 * ticks );
  LED_ON();

  while (1) {
    (void) rtems_rate_monotonic_period( period_id1, 2 * ticks );
    LED_OFF();

    (void) rtems_rate_monotonic_period( period_id2, 2 * ticks );
    LED_ON();
  }

  (void) rtems_task_delete( RTEMS_SELF );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_MAXIMUM_PERIODS           2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
