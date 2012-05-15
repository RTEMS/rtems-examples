/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>
#include <rtems/bspIo.h>
#include <libcpu/cpuModel.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  printk( "\n\n*** x86 DISPLAY CPU CAPABILITIES TEST ***\n" );
  printCpuInfo();
  printk( "\n\n*** END OF x86 DISPLAY CPU CAPABILITIES TEST ***\n" );

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
