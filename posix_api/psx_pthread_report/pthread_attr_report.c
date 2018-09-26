/*
 *  Program to print default pthread attributes
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

static void print_stack_info(pthread_attr_t *attr)
{
  int      rc;
  void    *addr;
  size_t   size;

  /*
   * pthread_attr_getstacksize() and pthread_attr_[sg]etstackaddr() are
   * obsolete.
   */

  rc = pthread_attr_getstack( attr, &addr, &size );
  assert( rc == 0 );
  printf( "Stack size: %zu\n", size );
  printf( "Stack address: %p\n", addr );

  rc = pthread_attr_getguardsize( attr, &size );
  assert( rc == 0 );
  printf( "Stack guard size: %zu\n", size );

}

static void print_inherit_scheduler(pthread_attr_t *attr)
{
  int      rc;
  int      sched;
  char    *s;

  rc = pthread_attr_getinheritsched( attr, &sched );
  assert( rc == 0 );

  switch( sched ) {
    case PTHREAD_INHERIT_SCHED:  s = "PTHREAD_INHERIT_SCHED";  break;
    case PTHREAD_EXPLICIT_SCHED: s = "PTHREAD_EXPLICIT_SCHED"; break;
    default:                     s = "UNKNOWN"; break;
  }

  printf( "Inherit scheduler: %s\n", s );
}

static void print_scope(pthread_attr_t *attr)
{
  int      rc;
  int      scope;
  char    *s;

  rc = pthread_attr_getscope( attr, &scope );
  assert( rc == 0 );

  switch( scope ) {
    case PTHREAD_SCOPE_SYSTEM:  s = "PTHREAD_SCOPE_SYSTEM";  break;
    case PTHREAD_SCOPE_PROCESS: s = "PTHREAD_SCOPE_PROCESS"; break;
    default:                     s = "UNKNOWN"; break;
  }

  printf( "Thread scope: %s\n", s );
}

static void print_detach_state(pthread_attr_t *attr)
{
  int      rc;
  int      state;
  char    *s;

  rc = pthread_attr_getdetachstate( attr, &state );
  assert( rc == 0 );

  switch( state ) {
    case PTHREAD_CREATE_JOINABLE: s = "PTHREAD_CREATE_JOINABLE";  break;
    case PTHREAD_CREATE_DETACHED: s = "PTHREAD_CREATE_DETACHED"; break;
    default:                      s = "UNKNOWN"; break;
  } 

  printf( "Thread detach state: %s\n", s );
}

static void print_sched_info(pthread_attr_t *attr)
{
  int                 rc;
  int                 policy;
  char               *s;
  struct sched_param  sched;

  rc = pthread_attr_getschedpolicy( attr, &policy );
  assert( rc == 0 );

  rc = pthread_attr_getschedparam( attr, &sched );
  assert( rc == 0 );

  switch( policy ) {
    case SCHED_OTHER:    s = "SCHED_OTHER";    break;
    case SCHED_FIFO:     s = "SCHED_FIFO";     break;
    case SCHED_RR:       s = "SCHED_RR";       break;
#if defined(SCHED_SPORADIC)
    case SCHED_SPORADIC: s = "SCHED_SPORADIC"; break;
#endif
    default:             s = "UNKNOWN";        break;
  } 

  printf( "Scheduler policy : %s\n", s );
  printf( "Scheduler priority : %d\n", sched.sched_priority );
#if defined(SCHED_SPORADIC)
  if ( policy == SCHED_SPORADIC ) {
    printf(
      "Sporadic low priority : %d\n"
      "Sporadic period: %lld:%ld\n"
      "Sporadic initial budget: %lld:%ld\n"
      "Sporadic max pending replenishments: %d\n",
      sched.sched_ss_low_priority,
      (long long) sched.sched_ss_repl_period.tv_sec,
      sched.sched_ss_repl_period.tv_nsec,
      (long long) sched.sched_ss_init_budget.tv_sec,
      sched.sched_ss_init_budget.tv_nsec,
      sched.sched_ss_max_repl
    );
  }
#endif
}

int main()
{
  pthread_attr_t  attr;
  int             rc;
  
  puts( "*** POSIX Thread Default Attributes Report ***" );

  rc = pthread_attr_init( &attr );
  assert( rc == 0 );

  print_stack_info( &attr );
  print_inherit_scheduler( &attr );
  print_scope( &attr );
  print_detach_state( &attr );
  print_sched_info( &attr );

  puts( "*** END OF POSIX Thread Default Attributes Report ***" );
  exit( 0 );
}
