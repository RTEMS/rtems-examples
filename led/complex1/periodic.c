/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * COPYRIGHT (c) 2026.
 * On-Line Applications Research Corporation (OAR).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <bsp.h>
#if !defined(BSP_SMALL_MEMORY)

#if defined(__rtems__)
#include <rtems.h>
#include <stdio.h>
#include "periodic.h"

#define MAX_PERIODIC 100

rtems_id PeriodicThreads_Ids[100];
rtems_id PeriodicThreads_Periods[100];

bool PeriodicThreads_initialized = FALSE;
bool PeriodicThreads_running = FALSE;

unsigned int PeriodicThreads_Count = 10;

int factorial( int i );

rtems_task PeriodicThreads_Body(
  rtems_task_argument index
)
{
  /* int i; */
  int t = index + 1;
  rtems_status_code s;
  rtems_name name;
  char buf[6];

  fprintf( stderr, "Periodic task %d starting\n", t );

  sprintf( buf, "PE%02x", index );
  name = rtems_build_name( buf[0], buf[1], buf[2], buf[3] );
  s = rtems_rate_monotonic_create( name, &PeriodicThreads_Periods[index] );
  if ( s ) {
    fprintf( stderr, "PeriodicThreads -- period create failed\n" );
    return;
  }

  /* this doesn't seem to dirty the stack :( */
  factorial(t * 100);
  while (1) {
    rtems_rate_monotonic_period( PeriodicThreads_Periods[index], t * 2 );
    /* for (i=0 ; i<(100000 * t) ; i++ ) ;  */
    rtems_task_wake_after( t );
  }
}

/* this doesn't seem to dirty the stack :( */
int factorial( int i )
{
  if ( i == 1 ) {
    return 1;
  }
  return i * (i - 1);
}

void PeriodicThreads_Initialize(void)
{
  int i;
  rtems_status_code s;
  rtems_name name;
  char buf[6];

  if ( PeriodicThreads_initialized )
    return;
  PeriodicThreads_running = FALSE;

  for( i=0 ; i<PeriodicThreads_Count ; i++ ) {
    sprintf( buf, "TA%02x", i );
    name = rtems_build_name( buf[0], buf[1], buf[2], buf[3] );
    s = rtems_task_create( name, 2, RTEMS_MINIMUM_STACK_SIZE * 4,
       RTEMS_DEFAULT_MODES, RTEMS_FLOATING_POINT, &PeriodicThreads_Ids[i] );
    if ( s ) {
      fprintf( stderr, "PeriodicThreads -- task create failed\n" );
      return;
    }

    s = rtems_task_start( PeriodicThreads_Ids[i], PeriodicThreads_Body, i );
    if ( s ) {
      fprintf( stderr, "PeriodicThreads -- task start failed\n" );
      return;
    }
  } 

  PeriodicThreads_running = TRUE;
  PeriodicThreads_initialized = TRUE;
}

void PeriodicThreads_Start(void)
{
  PeriodicThreads_Initialize();
}

void PeriodicThreads_Stop(void)
{
  rtems_status_code s;
  int i;

  if ( !PeriodicThreads_initialized )
    return;

  fprintf( stderr, "Stopping Periodic Threads\n" );
  for ( i=0 ; i<PeriodicThreads_Count ; i++ ) {
    s = rtems_rate_monotonic_delete( PeriodicThreads_Periods[i] );
    if ( s ) {
      fprintf( stderr, "PeriodicThreads -- period delete failed\n" );
      return;
    }

    s = rtems_task_delete( PeriodicThreads_Ids[i] );
    if ( s ) {
      fprintf( stderr, "PeriodicThreads -- task delete failed\n" );
      return;
    }
  }
  PeriodicThreads_running = FALSE;
  PeriodicThreads_initialized = FALSE;
}

#endif
#endif
