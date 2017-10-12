/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "../led.h"

pthread_mutex_t Mutex;
pthread_cond_t Condition;

void *Server(
  void * ignored
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

void *POSIX_Init(
  void *argument
)
{
  pthread_t thread_id;
  int       status;

  puts( "\n\n*** LED BLINKER -- sleep with pthread condition variables ***" );

  status = pthread_mutex_init( &Mutex, NULL );
  if (status) 
    fprintf( stderr, "mutex init (%d) \n", status );
  /* fprintf( stderr, "mutex id --> 0x%08x\n", Mutex ); */

  status = pthread_cond_init( &Condition, NULL );
  if (status) 
    fprintf( stderr, "cond init (%d) \n", status );
  /* fprintf( stderr, "cond id --> 0x%08x\n", Condition ); */

  status = pthread_create( &thread_id, NULL, Server, NULL );
  if (status) 
    fprintf( stderr, "pthread_create --> %d\n", status );

  for ( ; ; ) {

    status = sleep( 1 );

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

#define CONFIGURE_MAXIMUM_POSIX_THREADS 2

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* end of file */
