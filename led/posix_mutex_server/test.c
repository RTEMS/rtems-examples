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

void *Server(
  void * ignored
)
{
  int status;

  LED_INIT();

  for ( ; ; ) {

    status = pthread_mutex_lock( &Mutex );
    if (status)
      fprintf( stderr, "Server - lock did not work (%d)\n", status );

    LED_ON();

    status = sleep( 1 );

    status = pthread_mutex_unlock( &Mutex );
    if (status)
      fprintf( stderr, "Server - unlock did not work (%d)\n", status );

  }
  return NULL;
}

void *POSIX_Init(
  void *argument
)
{
#if defined(DEFINE_MUTEX_ATTRIBUTES)
  pthread_mutexattr_t mutex_attr;
#endif
  pthread_t thread_id;
  int status;

  puts( "\n\n*** LED BLINKER -- pthread_mutex ***" );

#if defined(DEFINE_MUTEX_ATTRIBUTES)
  status = pthread_mutexattr_init( &mutex_attr );
  status = pthread_mutexattr_setprotocol( &mutex_attr, PTHREAD_PRIO_INHERIT );
  status = pthread_mutexattr_setprioceiling( &mutex_attr, 128 );

  status = pthread_mutex_init( &Mutex, &mutex_attr );
#else
  status = pthread_mutex_init( &Mutex, NULL );
#endif
  if (status) 
    fprintf( stderr, "mutex init (%d) \n", status );
  /* fprintf( stderr, "Mutex id --> 0x%08x\n", Mutex ); */

  status = pthread_create( &thread_id, NULL, Server, NULL );
  if (status) 
    fprintf( stderr, "pthread_create --> %d\n", status );

  status = pthread_mutex_lock( &Mutex );
  if (status) 
    fprintf( stderr, "Main -- first lock did not work (%d)\n", status );

  for ( ; ; ) {

    LED_OFF();

    status = sleep( 1 );

    status = pthread_mutex_unlock( &Mutex );
    if (status)
      fprintf( stderr, "Main - unlock did not work (%d)\n", status );

    status = pthread_mutex_lock( &Mutex );
    if (status)
      fprintf( stderr, "Main - lock did not work (%d)\n", status );

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
