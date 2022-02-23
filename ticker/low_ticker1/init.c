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
  rtems_id          period_id;
  rtems_interval    ticks;
  struct timespec   uptime;

  (void) rtems_rate_monotonic_create(
    rtems_build_name( 'P', 'E', 'R', 0x30+task_index ),
    &period_id
  );

  ticks = task_index * 5 * rtems_clock_get_ticks_per_second();
  for ( ; ; ) {
    (void) rtems_rate_monotonic_period( period_id, ticks );

    (void) rtems_clock_get_uptime( &uptime );
    if ( uptime.tv_sec >= 35 ) {
      printk( "*** END OF LOW MEMORY CLOCK TICK TEST (PERIODs) ***\n" );
      rtems_shutdown_executive( 0 );
    }
    printk( "TA%d - rtems_clock_uptime - %lld:%ld\n", 
      task_index, (long long) uptime.tv_sec, uptime.tv_nsec 
    );
  }
}

static void *Init(
  uintptr_t ignored
)
{
  rtems_id          id;
  int               i;

  printk( "\n\n*** LOW MEMORY CLOCK TICK TEST (PERIODs) ***\n" );

  for (i=1 ; i<=3 ; i++ ) {
    (void) rtems_task_create(
      rtems_build_name( 'T', 'A', 0x30+1, ' ' ), 1, 0, RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES, &id
    );
    (void) rtems_task_start( id, Test_task, i );
  }

  while( 1 ) {
    ;
  }

  return NULL;
}

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_INIT

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_DISABLE_FILESYSTEM
#define CONFIGURE_DISABLE_NEWLIB_REENTRANCY
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 0

/*
 *  Reduce the stack as far as we think is safe on this architecture.
 *  On small CPUs, the recommended minimum may already be less than
 *  512 bytes so do not increase it.  But on at least the SPARC, we
 *  need more than 512 bytes of stack space.
 */
#if defined(__sparc__)
  #define CONFIGURE_MINIMUM_TASK_STACK_SIZE 1024
#elif (CPU_STACK_MINIMUM_SIZE > 512)
  #define CONFIGURE_MINIMUM_TASK_STACK_SIZE 512
#endif

/*
 * This parameter was eliminated after 4.11.
 */
#if (__RTEMS_MAJOR__ < 5)
#define CONFIGURE_TERMIOS_DISABLED
#endif


#define CONFIGURE_MAXIMUM_PRIORITY 15
#define CONFIGURE_DISABLE_CLASSIC_API_NOTEPADS
#define CONFIGURE_IDLE_TASK_BODY Init
#define CONFIGURE_IDLE_TASK_INITIALIZES_APPLICATION

#define CONFIGURE_MAXIMUM_TASKS             3
#define CONFIGURE_MAXIMUM_PERIODS           3

#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
