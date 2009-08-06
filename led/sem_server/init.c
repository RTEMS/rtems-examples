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
#include <assert.h>

#include "../../testmacros.h"
#include "../led.h"

/*
 *  Keep the names and IDs in global variables so another task can use them.
 */

rtems_id   Task_id[ 4 ];         /* array of task ids */
rtems_name Task_name[ 4 ];       /* array of task names */
rtems_id   Sem_id;

rtems_task Test_task(
  rtems_task_argument unused
)
{
  rtems_status_code status;

  LED_INIT();

  for ( ; ; ) {
    status = rtems_semaphore_obtain(
      Sem_id,
      RTEMS_DEFAULT_OPTIONS,
      RTEMS_NO_TIMEOUT
    );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "Task - obtain did not work\n", stderr );

    LED_OFF();

    status = rtems_task_wake_after( get_ticks_per_second() );
    assert( status == RTEMS_SUCCESSFUL );

    status = rtems_semaphore_release( Sem_id );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "Task - release did not work\n", stderr );
  }
}

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  puts( "\n\n*** LED BLINKER -- semaphore ping/pong ***" );

  Task_name[ 1 ] = rtems_build_name( 'T', 'A', '1', ' ' );

  status = rtems_semaphore_create(
    rtems_build_name( 'S', 'E', 'M', ' ' ),
    0,
    RTEMS_DEFAULT_ATTRIBUTES,
    0,
    &Sem_id
  );
  assert( status == RTEMS_SUCCESSFUL );

  status = rtems_task_create(
    Task_name[ 1 ], 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &Task_id[ 1 ]
  );
  assert( status == RTEMS_SUCCESSFUL );

  status = rtems_task_start( Task_id[ 1 ], Test_task, 1 );
  assert( status == RTEMS_SUCCESSFUL );

  while (1) {

    LED_ON();
    status = rtems_task_wake_after( get_ticks_per_second() );
    assert( status == RTEMS_SUCCESSFUL );

    status = rtems_semaphore_release( Sem_id );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "init - release did not work\n", stderr );

    status = rtems_semaphore_obtain(
      Sem_id,
      RTEMS_DEFAULT_OPTIONS,
      RTEMS_NO_TIMEOUT
    );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "init - obtain did not work\n", stderr );

  }

  status = rtems_task_delete( RTEMS_SELF );
  assert( status == RTEMS_SUCCESSFUL );
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS         4
#define CONFIGURE_MAXIMUM_SEMAPHORES    1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
