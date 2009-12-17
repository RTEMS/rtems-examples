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

rtems_id     Timer1;
rtems_id     Timer2;

rtems_timer_service_routine Timer_Routine( rtems_id id, void *ignored )
{
  rtems_status_code status;

  if ( id == Timer1 )
    LED_OFF();
  else
    LED_ON();

  status = rtems_timer_server_fire_after(
    id,
    2 * rtems_clock_get_ticks_per_second(),
    Timer_Routine,
    NULL
  );
}

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  puts( "\n\n*** LED BLINKER -- timer_server ***" );

  LED_INIT();

  status = rtems_timer_initiate_server(
    1, 
    RTEMS_MINIMUM_STACK_SIZE * 2,
    RTEMS_DEFAULT_ATTRIBUTES
  );

  if ( status != RTEMS_SUCCESSFUL )
    fputs( "timer create server failed\n", stderr );

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '1' ), &Timer1);
  if ( status != RTEMS_SUCCESSFUL )
    fputs( "Timer1 create failed\n", stderr );

  status = rtems_timer_create(rtems_build_name( 'T', 'M', 'R', '2' ), &Timer2);
  if ( status != RTEMS_SUCCESSFUL )
    fputs( "Timer2 create failed\n", stderr );

  Timer_Routine(Timer1, NULL);

  status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() );

  Timer_Routine(Timer2, NULL);

  status = rtems_task_delete( RTEMS_SELF );
}


/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS         2
#define CONFIGURE_MAXIMUM_TIMERS        2

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
