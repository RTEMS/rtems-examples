/*
 *  Program to print default POSIX rwlock attributes
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

static void print_shared(pthread_rwlockattr_t *attr)
{
  int   rc;
  int   shared;
  char *s;

  rc = pthread_rwlockattr_getpshared( attr, &shared );
  assert( rc == 0 );

  switch ( shared ) {
    case PTHREAD_PROCESS_PRIVATE: s = "PTHREAD_PROCESS_PRIVATE"; break;
    case PTHREAD_PROCESS_SHARED:  s = "PTHREAD_PROCESS_SHARED"; break;
    default:                      s = "UNKNOWN"; break;
  }

  printf( "Process shared: %s\n", s );
}

int main()
{
  pthread_rwlockattr_t  attr;
  int                  rc;
  
  puts( "*** POSIX RWLock Default Attributes Report ***" );

  rc = pthread_rwlockattr_init( &attr );
  assert( rc == 0 );

  print_shared( &attr );

  puts( "*** END OF POSIX RWLock Default Attributes Report ***" );
  exit( 0 );
}
