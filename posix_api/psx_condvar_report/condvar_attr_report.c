/*
 *  Program to print default POSIX condition variable attributes
 */

/*
 * Copyright 2018 Joel Sherrill (joel@rtems.org)
 *
 * This file's license is 2-clause BSD as in this distribution's LICENSE.2 file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include <assert.h>

static void print_shared(pthread_condattr_t *attr)
{
  int   rc;
  int   shared;
  char *s;

  rc = pthread_condattr_getpshared( attr, &shared );
  assert( rc == 0 );

  switch ( shared ) {
    case PTHREAD_PROCESS_PRIVATE: s = "PTHREAD_PROCESS_PRIVATE"; break;
    case PTHREAD_PROCESS_SHARED:  s = "PTHREAD_PROCESS_SHARED"; break;
    default:                      s = "UNKNOWN"; break;
  }

  printf( "Process shared: %s\n", s );
}

static void print_clock(pthread_condattr_t *attr)
{
  int         rc;
  clockid_t   clock;
  char       *s;

  rc = pthread_condattr_getclock( attr, &clock );
  assert( rc == 0 );
  switch ( clock ) {
    case CLOCK_MONOTONIC:          s = "CLOCK_MONOTONIC"; break;
    case CLOCK_PROCESS_CPUTIME_ID: s = "CLOCK_PROCESS_CPUTIME_ID"; break;
    case CLOCK_REALTIME:           s = "CLOCK_REALTIME"; break;
    case CLOCK_THREAD_CPUTIME_ID:  s = "CLOCK_THREAD_CPUTIME_ID"; break;
    default:                       s = "UNKNOWN"; break;
  }

  printf( "Clock: %s\n", s );
}

int main()
{
  pthread_condattr_t  attr;
  int                  rc;
  
  puts( "*** POSIX Condition Variable Default Attributes Report ***" );

  rc = pthread_condattr_init( &attr );
  assert( rc == 0 );

  print_shared( &attr );
  print_clock( &attr );

  puts( "*** END OF POSIX Condition Variable Default Attributes Report ***" );
  exit( 0 );
}
