/*
 *  Written by Petri Rokka <petri.rokka@tut.fi> based
 *  upon numerous RTEMS examples.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#define CONFIGURE_MINIMUM_TASK_STACK_SIZE (4*1024)
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_APPLICATION
#define CONFIGURE_MEMORY_OVERHEAD (2560)

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_MICROSECONDS_PER_TICK 1000
#define CONFIGURE_TICKS_PER_TIMESLICE 5

#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 50
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_SWAPOUT_TASK_PRIORITY 220
#define CONFIGURE_MAXIMUM_SEMAPHORES 2
#define CONFIGURE_MAXIMUM_TASKS 5

#define CONFIGURE_INIT_TASK_INITIAL_MODES (RTEMS_NO_PREEMPT | \
  RTEMS_NO_TIMESLICE | \
  RTEMS_ASR | \
  RTEMS_INTERRUPT_LEVEL(0))

#define CONFIGURE_INIT_TASK_STACK_SIZE (RTEMS_MINIMUM_STACK_SIZE * 20)
#define CONFIGURE_INIT_TASK_PRIORITY 50
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS (RTEMS_MINIMUM_STACK_SIZE * 3)

#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 10

#define CONFIGURE_SHELL_COMMANDS_INIT

#include <rtems/shell.h>

int nbench_main(int argc, char **argv);

#define CONFIGURE_SHELL_COMMANDS_ALL

#include <rtems/shellconfig.h>

#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES (RTEMS_FLOATING_POINT)
#define CONFIGURE_STACK_CHECKER_ENABLED
#include <rtems/confdefs.h>

#include <stdio.h>
#include <rtems.h>

rtems_task task_nbench(rtems_task_argument arg)
{
  char *args[1] = {"./nbench"};
  nbench_main(1, args);
}

rtems_task Init(rtems_task_argument arg)
{
  rtems_status_code   sc;
  rtems_id            task_id_nbench;
  char               *args[1] = {"./nbench"};
  rtems_interval      tickspersec;

  /*
   *  Task uses about 22K on x86
   */
  sc = rtems_task_create(
    rtems_build_name('N','B','M','2'),
    51,
    64*1024,
    RTEMS_DEFAULT_MODES,
    RTEMS_FLOATING_POINT,
    &task_id_nbench);
  if ( sc )
    printf( "unable to create NBM2\n" );

  sc = rtems_task_start( task_id_nbench, task_nbench, 0 );
  if ( sc )
    printf( "unable to start NBM2\n" );

  tickspersec = rtems_clock_get_ticks_per_second();

  printf("clocks per sec: %ld\n", tickspersec);
  printf("Starting Shell\n");

  rtems_shell_main_loop(NULL);

  rtems_task_delete( RTEMS_SELF );
}
