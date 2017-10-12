/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "../../testmacros.h"
#include "../led.h"

pthread_mutex_t Mutex;
pthread_cond_t Condition;

void *POSIX_Init(
  void *argument
)
{
  int status;
  int count = 0;

  LED_INIT();

  status = pthread_mutex_lock( &Mutex );
  if ( status )
    fprintf( stderr, "Server - lock did not work (%d)\n", status );

  for (count=0; ; count++) {

    status = pthread_cond_wait( &Condition, &Mutex );
    if ( status )
      fprintf( stderr, "Server - condition wait did not work (%d)\n", status );

    if ((count % 2) == 0)
      LED_OFF();
    else
      LED_ON();
  }
  return NULL;
}

rtems_task Init(
  rtems_task_argument argument
)
{
  int               status;
  rtems_status_code rt_status;
  rtems_id          period_id;
  rtems_interval    ticks;

  puts(
    "\n\n*** LED BLINKER -- rate monotonic period with "
    "pthread condition variables ***"
  );

  status = pthread_mutex_init( &Mutex, NULL );
  if (status) 
    fprintf( stderr, "mutex init (%d) \n", status );
  /* fprintf( stderr, "mutex id --> 0x%08x\n", Mutex ); */

  status = pthread_cond_init( &Condition, NULL );
  if (status) 
    fprintf( stderr, "cond init (%d) \n", status );
  /* fprintf( stderr, "cond id --> 0x%08x\n", Condition ); */

  rt_status = rtems_rate_monotonic_create(
    rtems_build_name( 'P', 'E', 'R', '1' ),
    &period_id
  );
  if ( rt_status )
    fprintf( stderr, "Period - did not work (%d)\n", rt_status );

  ticks = rtems_clock_get_ticks_per_second();

  for ( ; ; ) {

    rt_status = rtems_rate_monotonic_period( period_id, ticks );
    if ( status )
      fprintf( stderr, "Period - did not work (%d)\n", status );

    status = pthread_cond_signal( &Condition );
    if ( status )
      fprintf( stderr, "Condition - signal did not work (%d)\n", status );
  }
  exit( 0 );
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_TASKS   1
#define CONFIGURE_MAXIMUM_PERIODS 1

#define CONFIGURE_MAXIMUM_POSIX_THREADS 2

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* end of file */
