/*
 *  Demonstrate accessing Boot Command Line provided by BSP
 */

#include <bsp.h>
#include <rtems/bspcmdline.h>

#include <stdlib.h>
#include <stdio.h>

void print_arg(
  const char *param
)
{
  const char *p;
  char        value[80];
  size_t      length;

  printf( "\nLooking for param=(%s)\n", param );

  length = sizeof(value);
  p = rtems_bsp_cmdline_get_param( param, value, length );
  if ( !p ) {
    printf( "Did not locate param=(%s)\n", param );
    return;
  }

  p = rtems_bsp_cmdline_get_param_rhs( param, value, length );
  if ( !p ) {
    printf( "param=(%s) has no right hand side\n", param );
  } else {
    printf( "param=(%s) has right hand side of %s\n", param, p );
  }
  
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  const char *bspcmdline;

  printf( "\n\n*** BSP Command Demonstration ***\n" );

  /*
   *  Let's see if this BSP provided one at all
   */
  bspcmdline = rtems_bsp_cmdline_get();
  if ( !bspcmdline ) {
    puts( "BSP does not have a boot command line" );
    goto demo_over;
  }

  printf( "BSP has a boot command line:\n" "%s\n", bspcmdline );

  /*
   *  The PC386 has some defined arguments.  Did we see any of those?
   */ 
  print_arg( "--console" );
  print_arg( "--ide" );
  print_arg( "--ide-show" );
  

demo_over:
  printf( "*** END OF BSP Command Line Demonstration ***\n" );
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
