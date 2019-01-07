/*
 *  COPYRIGHT (c) 1989-2012.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

//Adapted from Lawrence Livermore Pthread Tutorial #1
//https://computing.llnl.gov/tutorials/pthreads

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <bsp.h> /* for device driver prototypes */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS    5

/* forward declarations to avoid warnings */
void *POSIX_Init(void *argument);
void *Print_Hello(void *threadid);

void *Print_Hello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);

   //You must include the return 'NULL;' in your threads to make the compiler happy
   pthread_exit(NULL);
   return NULL;
}


void *POSIX_Init(void *argument)
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;

   //Creates threads
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, Print_Hello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }


   //Joins the child threads up to the main one.
   for(t=0; t<NUM_THREADS; t++){
      pthread_join(threads[t],NULL);
   }
   /* Last thing that main() should do */
   exit(0);
}

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1
#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NOTICE: These configuration variable MUST be initialized before using Pthreads. The will not work if you do not.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CONFIGURE_MAXIMUM_POSIX_THREADS 6                                                                    /////
#define CONFIGURE_POSIX_INIT_THREAD_TABLE                                                                    /////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
