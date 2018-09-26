/*
 *  Program to print POSIX Scheduler Characteristics
 */

/*
 * Copyright 2018 Joel Sherrill (joel@rtems.org)
 *
 * This file's license is 2-clause BSD as in this distribution's LICENSE.2 file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
/* FreeBSD 10 needs this for struct timespec which seems wrong */
#include <time.h>

void print_sched_info(
  char *s,
  int policy
)
{
  int min, max, levels;
  struct timespec t;

  printf( "Information on %s\n", s );
  min = sched_get_priority_min( policy );
  max = sched_get_priority_max( policy );
  (void) sched_rr_get_interval( 1, &t );
  levels = abs(max - min + 1);
  printf( "\tSupports %d priority levels (%d - %d)\n", levels, min, max  );
  if ( levels >= 32 )
    printf( "\tImplementation is compliant on priority levels\n");
  else
    printf( "\tImplementation is NOT compliant on priority levels\n" );

  printf( "\tScheduling quantum is %ld seconds and %ld nanoseconds\n",
              (long)t.tv_sec, (long)t.tv_nsec );
  puts( "" );
}

int main()
{
  puts( "*** POSIX Scheduler Characteristics Report ***" );
  print_sched_info( "SCHED_OTHER", SCHED_OTHER );
  print_sched_info( "SCHED_FIFO", SCHED_FIFO );
  print_sched_info( "SCHED_RR", SCHED_RR );
#if defined(SCHED_SPORADIC)
  print_sched_info( "SCHED_SPORADIC", SCHED_SPORADIC );
#else
  printf( "SCHED_SPORADIC is not supported\n" );
#endif
  puts( "*** END OF POSIX Scheduler Characteristics Report ***" );

  exit( 0 );
}
