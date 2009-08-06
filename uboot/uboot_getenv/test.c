/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>

#include <stdlib.h>
#include <stdio.h>

#if defined(HAS_UBOOT)
  void test_getenv(
    const char *name
  )
  {
    const char *bsp_uboot_getenv(
      const char *name
    );
    const char *rhs = bsp_uboot_getenv( name );

    printf( "%s=%s\n", name, ((rhs) ? rhs : NULL));
  }
#endif

rtems_task Init(
  rtems_task_argument ignored
)
{
  puts( "\n\n*** U-Boot Get Environment Variable Test ***" );
  #if defined(HAS_UBOOT)
    test_getenv( "ipaddr" );
    test_getenv( "baudrate" );
    test_getenv( "bootcmd" );
    test_getenv( "badname" );
  #else
    puts( "This BSP does not have U-Boot." );
  #endif
  puts( "*** END of U-Boot Get Environment Variable Test ***" );

  exit( 0 );
}

/* configuration information */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* end of file */
