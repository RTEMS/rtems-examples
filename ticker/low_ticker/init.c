/*
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <stdlib.h>

#include <bsp.h>
#include <rtems/bspIo.h>

rtems_task Test_task(
  rtems_task_argument task_index
)
{
  rtems_time_of_day time;
  rtems_status_code status;

  for ( ; ; ) {
    status = rtems_clock_get_tod( &time );
    if ( time.second >= 35 ) {
      printk( "*** END OF LOW MEMORY CLOCK TICK TEST ***\n" );
      rtems_shutdown_executive( 0 );
    }
    printk( "TA%d %s%02d:%02d:%02d   %02d/%02d/%04d\n",
      task_index, " - rtems_clock_get_tod - ", 
      (int) time.hour, (int) time.minute, (int) time.second,
      (int) time.month, (int) time.day, (int) time.year );
    status = rtems_task_wake_after(
      task_index * 5 * rtems_clock_get_ticks_per_second()
    );
  }
}

static void *Init(
  uintptr_t ignored
)
{
  rtems_status_code status;
  rtems_time_of_day time;
  rtems_id          id;
  int               i;

  printk( "\n\n*** LOW MEMORY CLOCK TICK TEST ***\n" );

  time.year   = 1988;
  time.month  = 12;
  time.day    = 31;
  time.hour   = 9;
  time.minute = 0;
  time.second = 0;
  time.ticks  = 0;

  status = rtems_clock_set( &time );

  for (i=1 ; i<=3 ; i++ ) {
    status = rtems_task_create(
      rtems_build_name( 'T', 'A', 0x30+1, ' ' ), 1, 0, RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES, &id
    );
    status = rtems_task_start( id, Test_task, i );
  }

  while( 1 )
    ;
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 0
#define CONFIGURE_MINIMUM_TASK_STACK_SIZE 512
#define CONFIGURE_MAXIMUM_PRIORITY 15
#define CONFIGURE_DISABLE_CLASSIC_API_NOTEPADS
#define CONFIGURE_IDLE_TASK_BODY Init
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION

#define CONFIGURE_MAXIMUM_TASKS             3

/*
 * This parameter was eliminated after 4.11.
 */
#if (__RTEMS_MAJOR__ < 5)
#define CONFIGURE_TERMIOS_DISABLED
#endif

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
