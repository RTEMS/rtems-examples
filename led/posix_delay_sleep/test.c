/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../led.h"

void *POSIX_Init(
  void *argument
)
{
  puts( "\n\n*** LED BLINKER -- sleep ***" );

  LED_INIT();

  while (1) {

    (void) sleep( 1 );
    LED_OFF();
    (void) sleep( 1 );
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
