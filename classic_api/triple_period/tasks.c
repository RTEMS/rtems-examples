/* 
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */
/*  updated for triple test, 20003/11/06, Erik Adli */

#include "system.h"
#include <stdio.h>
#include <stdlib.h>

/* CPU usage and Rate monotonic manger statistics */
#include "rtems/cpuuse.h"

// Periods for the various tasks [seconds]
#define PERIOD_TASK_ABSOLUTE           1
#define PERIOD_TASK_RATE_MONOTONIC     2
#define PERIOD_TASK_RELATIVE           3



// TASK 1
//
// * Absolute timing for task 1
// * CPU usage statistics 
// * Demo end criteria
//
rtems_task Task_Absolute_Period(
  rtems_task_argument unused
)
{
  rtems_time_of_day time;
  rtems_status_code status;
  uint32_t          ticks_since_boot;
  uint32_t          count;

  count = 0;
  rtems_cpu_usage_reset();

  while( 1 ) {
    status = rtems_clock_get_tod( &time );
    count++;

    // sets end criteria for demo application (60 seconds)
    if ( time.second >= 60 ) {
      puts( "*** END OF PERIODIC TASKING TRIPLE TEST ***" );
      exit( 0 );
    }

    printf(
      "\n\nTask 1 - activating every %d second using "
      "absolute time (rtems_task_wake_when)\n",
      PERIOD_TASK_ABSOLUTE
    );
    print_time( " - rtems_clock_get_tod - ", &time, "\n" );
    ticks_since_boot = rtems_clock_get_ticks_since_boot();
    printf(" - Ticks since boot: %" PRIu32 "\n", ticks_since_boot);

    rtems_time_of_day time;

    time.year   = 1988;
    time.month  = 12;
    time.day    = 31;
    time.hour   = 9;
    time.minute = 0;
    time.second = count * PERIOD_TASK_ABSOLUTE;  // Every  N1 seconds
    time.ticks  = 0;                // NB!! 'ticks' is don't care ( = does not work); rtems_task_wait_when has granularity of 1 second ( "taskwakewhen.c" nullifies time.ticks )

    status = rtems_task_wake_when( &time );

    // dump CPU usage every 5th period
    if( 0 == (count % 5) ) {
      //CPU_usage_Dump( );  // UNCOMMENT ME FOR WORKING CPU_usage_Dump
    }
  }
};


// TASK 2
//
// * RM schdeling for task 2
// * Rate Monotonic usage statistics
//
rtems_task Task_Rate_Monotonic_Period(
  rtems_task_argument unused
)
{
  rtems_time_of_day time;
  rtems_status_code status;
  uint32_t          ticks_since_boot;
  rtems_name        my_period_name; 
  rtems_id          RM_period;
  bool              is_RM_created = 0;
  uint32_t          count;

  count = 0;

  while( 1 ) {
    status = rtems_clock_get_tod( &time );
    count++;

    printf( "\n\nTask 2 - activating every %d second using rate monotonic manager to schedule (rtems_rate_monotonic_period)\n", PERIOD_TASK_RATE_MONOTONIC);
    print_time( " - rtems_clock_get_tod - ", &time, "\n" );
    ticks_since_boot = rtems_clock_get_ticks_since_boot();
    printf(" - Ticks since boot: %" PRIu32 "\n", ticks_since_boot);

    if( TRUE != is_RM_created ) {
      count = 0;
      my_period_name = rtems_build_name( 'P', 'E', 'R', '1' );
      status = rtems_rate_monotonic_create( my_period_name, &RM_period );
      if( RTEMS_SUCCESSFUL != status ) {
	printf("RM failed with status: %d\n", status);
	exit(1);
      }
      // Initiate RM period -- every N2 seconds
      status = rtems_rate_monotonic_period(
        RM_period,
        rtems_clock_get_ticks_per_second() * PERIOD_TASK_RATE_MONOTONIC
      );
      if( RTEMS_SUCCESSFUL != status ) {
	printf("RM failed with status: %d\n", status);
	exit(1);
      }

      is_RM_created = TRUE;
    }
    // Block until RM period has expired -- every N2 seconds
    status = rtems_rate_monotonic_period(
      RM_period,
      rtems_clock_get_ticks_per_second() * PERIOD_TASK_RATE_MONOTONIC
    );
    if( RTEMS_SUCCESSFUL != status ) {
      if( RTEMS_TIMEOUT != status ) {
	printf("RM missed period!\n");
      }
      printf("RM failed with status: %d\n", status);
      exit(1);
    }

    // dump Rate Monotonic usage every 5th period
    if( 0 == (count % 5) ) {
      ; // rtems_rate_monotonic_report_statistics();
    }
  }
}


// TASK 3
// 
// * relative delay for task 3
//
rtems_task Task_Relative_Period(
  rtems_task_argument unused
)
{
  rtems_time_of_day time;
  rtems_status_code status;
  uint32_t          ticks_since_boot;

  while( 1 ) {
    status = rtems_clock_get_tod( &time );

    printf(
      "\n\nTask 3 - activating after every %d second using relative "
        "time (rtems_task_wake_after)\n",
      PERIOD_TASK_RELATIVE
    );
    print_time( " - rtems_clock_get_tod - ", &time, "\n" );
    ticks_since_boot = rtems_clock_get_ticks_since_boot();
    // Note how the ticks are drifting with this method
    printf(" - Ticks since boot: %" PRIu32 "\n", ticks_since_boot);

    // Every N3 seconds
    status = rtems_task_wake_after(
      rtems_clock_get_ticks_per_second() * PERIOD_TASK_RELATIVE
    );
  }
}

