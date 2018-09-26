/*
 *  Program to print default POSIX barrier attributes
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

#ifndef OS_DOES_NOT_SUPPORT_BARRIERS

static void print_shared(pthread_barrierattr_t *attr)
{
  int   rc;
  int   shared;
  char *s;

  rc = pthread_barrierattr_getpshared( attr, &shared );
  assert( rc == 0 );

  switch ( shared ) {
    case PTHREAD_PROCESS_PRIVATE: s = "PTHREAD_PROCESS_PRIVATE"; break;
    case PTHREAD_PROCESS_SHARED:  s = "PTHREAD_PROCESS_SHARED"; break;
    default:                      s = "UNKNOWN"; break;
  }

  printf( "Process shared: %s\n", s );
}
#endif

int main()
{
#ifndef OS_DOES_NOT_SUPPORT_BARRIERS
  pthread_barrierattr_t  attr;
#endif
  int                  rc;
  
  puts( "*** POSIX Barrier Default Attributes Report ***" );

#ifndef OS_DOES_NOT_SUPPORT_BARRIERS
  rc = pthread_barrierattr_init( &attr );
  assert( rc == 0 );

  print_shared( &attr );
#else
  printf( "Barriers are not supported\n" );
#endif

  puts( "*** END OF POSIX Barrier Default Attributes Report ***" );
  exit( 0 );
}
