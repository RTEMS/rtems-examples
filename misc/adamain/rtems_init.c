/* 
 *  COPYRIGHT (c) 1989-2000.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be found in
 *  the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <bsp.h>

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#define DEBUG

#if defined(MAIN_USE_NETWORKING)
  #include <rtems/rtems_bsdnet.h>
  #include "networkconfig.h"
#endif
#if defined(ENABLE_UNTAR_ROOT_FILESYSTEM)
  #include <rtems/untar.h>
  /*
   *  The tarfile image is built automatically externally.
   */
  #include "FilesystemImage.h"
#endif

/*
 *  Using GNAT's Distributed Systems Annex support requires
 *  each node in the system to have different pid's.  In a
 *  single CPU RTEMS system, it is always one.  This lets the
 *  user override the RTEMS default.
 */
#ifdef GNAT_PID
  #include <unistd.h>

  pid_t getpid()
  {
    return GNAT_PID;
  }
#endif

/*
 * Set up first argument
 */
static int   argc     = 1;
static char  arg0[20] = "rtems";
static char *argv[20] = { arg0 };

#if defined(MAIN_USE_REQUIRES_COMMAND_LINE)

#define COMMAND_LINE_MAXIMUM 200

#include <stdio.h>
#include <ctype.h>

void parse_arguments(
  char   *buffer,
  size_t  maximum_length
)
{
  char   *cp;
  char   *linebuf = buffer;
  size_t  length;

  for (;;) {

    #if defined(MAIN_COMMAND_LINE)
      strncpy (linebuf, MAIN_COMMAND_LINE, maximum_length);
    #else
      /*
       * Read a line
       */
      printf (">>> %s ", argv[0]);
      fflush (stdout);
      fgets (linebuf, maximum_length, stdin);

      length = strnlen( linebuf, maximum_length );
      if ( linebuf[length - 1] == '\n' || linebuf[length - 1] == '\r' ) {
	 linebuf[length - 1] = '\0';
      }
    #endif

    /*
     * Break line into arguments
     */
    cp = linebuf;
    for (;;) {
      while (isspace (*cp))
        *cp++ = '\0';
      if (*cp == '\0')
        break;
      if (argc >= ((sizeof argv / sizeof argv[0]) - 1)) {
        printf ("Too many arguments.\n");
        argc = 0;
        break;
      }
      argv[argc++] = cp;
      while (!isspace (*cp)) {
        if (*cp == '\0')
          break;
        cp++;
      }
    }
    if (argc > 1) {
      argv[argc] = NULL;
      break;
    }
    printf ("You must give some arguments!\n");
  }

  #if defined(DEBUG_COMMAND_LINE_ARGUMENTS)
    {
      int   i;
      for (i=0; i<argc ; i++ ) {
        printf( "argv[%d] = ***%s***\n", i, argv[i] );
      }
      printf( "\n" );
    }
  #endif 
}


#endif

/*
 *  By having the POSIX_Init thread create a second thread just
 *  to invoke gnat_main, we can override all default attributes
 *  of the "Ada environment task".  Otherwise, we would be
 *  stuck with the defaults set by RTEMS.
 */
 
void *start_gnat_main( void * argument )
{
  extern int gnat_main ( int argc, char **argv, char **envp );

  #if defined(DEBUG)
    printk( "Entering gnat_main\n" );
  #endif

  #if defined(MAIN_USE_REQUIRES_COMMAND_LINE)
    /*
     * This is scoped to match the Ada program.
     */
    char command_line[ COMMAND_LINE_MAXIMUM ];

    parse_arguments( command_line, COMMAND_LINE_MAXIMUM );
  #endif

  #if defined(DEBUG)
    printk( "Invoking gnat_main\n" );
  #endif

  (void) gnat_main ( argc, argv, 0 );

  #if defined(DEBUG)
    printk( "Exiting Ada application\n" );
  #endif

  exit( 0 );

  return 0;
}

#ifndef GNAT_MAIN_STACKSPACE
  #define GNAT_MAIN_STACKSPACE 0
#endif

void *POSIX_Init( void *argument )
{
  pthread_t       thread_id;
  pthread_attr_t  attr;
  size_t          stacksize;
  int             status;
  extern  size_t  _ada_pthread_minimum_stack_size();

  #if defined(DEBUG)
    printk( "Entering POSIX_Init\n" );
  #endif

  #if defined(ENABLE_UNTAR_ROOT_FILESYSTEM)
    printk("Loading filesystem image\n");
    status = Untar_FromMemory( (char *)FilesystemImage, FilesystemImage_size );
  #endif

  #if defined(MAIN_USE_NETWORKING)
    printk("Initializing Network\n");
    rtems_bsdnet_initialize_network ();
    rtems_bsdnet_show_inet_routes ();
  #endif

  #if defined(MAIN_CALL_C_INITIALIZE_APPLICATION)
  {
    extern void initialize_application();
    printk("Invoking C Application Initialization\n");
    initialize_application();
  }
  #endif

  /*
   * Now create the thread that will be the GNAT Ada main.
   */
  #if defined(DEBUG)
    printk( "Creating GNAT Main thread\n" );
  #endif

  status = pthread_attr_init( &attr );
  assert( !status );

  stacksize = GNAT_MAIN_STACKSPACE * 1024;
  if ( stacksize < _ada_pthread_minimum_stack_size() )
    stacksize = _ada_pthread_minimum_stack_size();

  status = pthread_attr_setstacksize( &attr, stacksize );
  assert( !status );

  attr.schedpolicy = SCHED_RR;
  attr.schedparam.sched_priority = 122;
  status = pthread_create( &thread_id, &attr, start_gnat_main, NULL );
  assert( !status );

  #if defined(DEBUG)
    printk( "Exiting POSIX_Init\n" );
  #endif

  pthread_exit( 0 );
  return 0;
}

/* configuration information */

#if defined(HAS_EXTRA_CONFIGURATION)
#include "config.h"
#endif

/* Standard output and a clock tick so time passes */
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#if 1
/* We need to be able to create sockets */
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS	20

/* We need the full IMFS to pass the full ACATS */
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

/* This is overkill but is definitely enough to run the network stack */
#define CONFIGURE_MAXIMUM_TASKS                         20
#define CONFIGURE_MAXIMUM_SEMAPHORES                    20

/* We want a clock tick every millisecond */
#define CONFIGURE_MICROSECONDS_PER_TICK RTEMS_MILLISECONDS_TO_MICROSECONDS(1)
#endif

/* The initialization task is a POSIX Initialization thread with default
 * attributes.
 */
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#if 1
/* We are using GNAT/RTEMS with a maximum of 20 Ada tasks and no fake Ada
 * tasks.
 * NOTE: A fake Ada task is a task created outside the Ada run-time that
 * calls into Ada.
 */
#define CONFIGURE_GNAT_RTEMS
#define CONFIGURE_MAXIMUM_ADA_TASKS      20

#if !defined(CONFIGURE_MAXIMUM_FAKE_ADA_TASKS)
  #define CONFIGURE_MAXIMUM_FAKE_ADA_TASKS 0
#endif

#if !defined(ADA_APPLICATION_NEEDS_EXTRA_MEMORY)
  #define ADA_APPLICATION_NEEDS_EXTRA_MEMORY 0
#endif

/* Account for any extra task stack size */
#define CONFIGURE_MEMORY_OVERHEAD \
  (ADA_APPLICATION_NEEDS_EXTRA_MEMORY + GNAT_MAIN_STACKSPACE)

/* Make sure the C Program Heap and Workspace are zeroed for GNAT */
#endif
#if 1
#define CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY TRUE
#endif

#define CONFIGURE_INIT

void _flush_cache() {}
#include <rtems/confdefs.h>
