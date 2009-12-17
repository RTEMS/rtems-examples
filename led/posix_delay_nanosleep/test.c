/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../led.h"

void *POSIX_Init(
  void *argument
)
{
  struct timespec delay;

  puts( "\n\n*** LED BLINKER -- nanosleep ***" );

  LED_INIT();

  delay.tv_sec = 1;
  delay.tv_nsec = 0;
  while (1) {

    (void) nanosleep( &delay, NULL );
    LED_OFF();
    (void) nanosleep( &delay, NULL );
    LED_ON();

  }
  exit( 0 );
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_MAXIMUM_POSIX_THREADS 1

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* end of file */
