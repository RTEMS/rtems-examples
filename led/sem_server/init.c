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
#include <assert.h>

#include "../led.h"

rtems_id   Sem_id;

rtems_task Test_task(
  rtems_task_argument unused
)
{
  rtems_status_code status;

  for ( ; ; ) {

    /* Semaphore not available, ensured to block */
    status = rtems_semaphore_obtain(
      Sem_id,
      RTEMS_DEFAULT_OPTIONS,
      RTEMS_NO_TIMEOUT
    );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "Task - obtain did not work\n", stderr );

    LED_ON();

    status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() );
    assert( status == RTEMS_SUCCESSFUL );

    /* Transfers semaphore to Init task */
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
  rtems_id          task_id;
  rtems_name        task_name;

  puts( "\n\n*** LED BLINKER -- semaphore ping/pong ***" );

  LED_INIT();

  status = rtems_semaphore_create(
    rtems_build_name( 'S', 'E', 'M', ' ' ),
    0,  /* created locked */
    RTEMS_DEFAULT_ATTRIBUTES,
    0,
    &Sem_id
  );
  assert( status == RTEMS_SUCCESSFUL );

  task_name = rtems_build_name( 'T', 'A', '1', ' ' );

  status = rtems_task_create(
    task_name, 1, RTEMS_MINIMUM_STACK_SIZE * 2, RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES, &task_id
  );
  assert( status == RTEMS_SUCCESSFUL );

  status = rtems_task_start( task_id, Test_task, 1 );
  assert( status == RTEMS_SUCCESSFUL );

  while (1) {

    LED_OFF();
    status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() );
    assert( status == RTEMS_SUCCESSFUL );

    /* Transfers semaphore to TA1 */
    status = rtems_semaphore_release( Sem_id );
    if ( status != RTEMS_SUCCESSFUL )
      fputs( "init - release did not work\n", stderr );

    /* Semaphore not available, ensured to block */
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
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_TASKS         2
#define CONFIGURE_MAXIMUM_SEMAPHORES    1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/****************  END OF CONFIGURATION INFORMATION  ****************/
