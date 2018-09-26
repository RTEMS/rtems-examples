/*
 *  Program to print default POSIX mutex attributes
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

static void print_shared(pthread_mutexattr_t *attr)
{
  int   rc;
  int   shared;
  char *s;

  rc = pthread_mutexattr_getpshared( attr, &shared );
  assert( rc == 0 );

  switch ( shared ) {
    case PTHREAD_PROCESS_PRIVATE: s = "PTHREAD_PROCESS_PRIVATE"; break;
    case PTHREAD_PROCESS_SHARED:  s = "PTHREAD_PROCESS_SHARED"; break;
    default:                      s = "UNKNOWN"; break;
  }

  printf( "Process shared: %s\n", s );
}

static void print_type(pthread_mutexattr_t *attr)
{
  int   rc;
  int   type;
  char *s;

#ifndef PTHREAD_MUTEX_ROBUST
  puts( "PTHREAD_MUTEX_ROBUST is not supported" );
#endif
#ifndef PTHREAD_MUTEX_STALLED
  puts( "PTHREAD_MUTEX_STALLED is not supported" );
#endif
#if (PTHREAD_MUTEX_DEFAULT == PTHREAD_MUTEX_NORMAL)
  puts( "PTHREAD_MUTEX_DEFAULT == PTHREAD_MUTEX_NORMAL" );
#endif

  /*
   * NOTE: The FACE Technical Standard defines multiple POSIX profiles.
   *       Some of the profiles do not include pthread_mutexattr_gettype().
   */
#ifdef OS_DOES_NOT_SUPPORT_PTHREAD_MUTEXATTR_GETTYPE
  s = "pthread_mutexattr_gettype() is not supported";
#else
  rc = pthread_mutexattr_gettype( attr, &type );
  assert( rc == 0 );
  switch ( type ) {
#if (PTHREAD_MUTEX_DEFAULT != PTHREAD_MUTEX_NORMAL)
    case PTHREAD_MUTEX_DEFAULT:    s = "PTHREAD_MUTEX_DEFAULT"; break;
#endif
    case PTHREAD_MUTEX_ERRORCHECK: s = "PTHREAD_MUTEX_ERRORCHECK"; break;
    case PTHREAD_MUTEX_NORMAL:     s = "PTHREAD_MUTEX_NORMAL"; break;
    case PTHREAD_MUTEX_RECURSIVE:  s = "PTHREAD_MUTEX_RECURSIVE"; break;
#ifdef PTHREAD_MUTEX_ROBUST
    case PTHREAD_MUTEX_ROBUST:     s = "PTHREAD_MUTEX_ROBUST"; break;
#endif
#ifdef PTHREAD_MUTEX_STALLED
    case PTHREAD_MUTEX_STALLED:    s = "PTHREAD_MUTEX_STALLED"; break;
#endif
    default:                       s = "UNKNOWN"; break;
  }
#endif

  printf( "Type: %s\n", s );
}

static void print_protocol(pthread_mutexattr_t *attr)
{
#if defined(__CYGWIN__)
    /* Cygwin does not support the protocols */
   puts( "Mutex protocols not supported" ); 
#else
  int   rc;
  int   protocol;
  char *s;

  rc = pthread_mutexattr_getprotocol( attr, &protocol );
  assert( rc == 0 );
  switch ( protocol ) {
    /* XXX something is not right on CentOS. The constants are not available.*/
    /* XXX Figure it out */
#ifdef __linux__
    case 0: s = "PTHREAD_PRIO_NONE"; break;
    case 1: s = "PTHREAD_PRIO_INHERIT"; break;
    case 2: s = "PTHREAD_PRIO_PROTECT"; break;
#else
    case PTHREAD_PRIO_NONE:    s = "PTHREAD_PRIO_NONE"; break;
    case PTHREAD_PRIO_INHERIT: s = "PTHREAD_PRIO_INHERIT"; break;
    case PTHREAD_PRIO_PROTECT: s = "PTHREAD_PRIO_PROTECT"; break;
#endif
    default:                   s = "UNKNOWN"; break;
  }

  printf( "Protocol: %s\n", s );
  /*
   * Assuming that PTHREAD_PRIO_CEILING is not used as default, so
   * no need to print ceiling.
   */
#endif
}

int main()
{
  pthread_mutexattr_t  attr;
  int                  rc;
  
  puts( "*** POSIX Mutex Default Attributes Report ***" );

  rc = pthread_mutexattr_init( &attr );
  assert( rc == 0 );

  print_type( &attr );
  print_shared( &attr );
  print_protocol( &attr );

  puts( "*** END OF POSIX Mutex Default Attributes Report ***" );
  exit( 0 );
}
