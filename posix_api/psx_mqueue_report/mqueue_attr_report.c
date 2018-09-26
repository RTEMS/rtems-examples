/*
 *  Program to print default POSIX message queue attributes
 */

/*
 * Copyright 2018 Joel Sherrill (joel@rtems.org)
 *
 * This file's license is 2-clause BSD as in this distribution's LICENSE.2 file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>  
#include <mqueue.h>

static void print_attr(mqd_t mqd)
{
  int    rc;
  struct mq_attr attr;

  rc = mq_getattr( mqd, &attr );
  assert( rc == 0 );

  printf( "mq_maxmsg: %ld\n", attr.mq_maxmsg );
  printf( "mq_msgsize: %ld\n", attr.mq_msgsize );
}

int main()
{
  mqd_t  mqd;
  
  puts( "*** POSIX Message Queue Default Attributes Report ***" );

  mqd = mq_open( "/testq", O_CREAT|O_RDWR, 0777, NULL );
  if ( mqd == (mqd_t) -1 ) {
    perror("mq_open" );
  }
  assert( mqd != (mqd_t) -1 );

  print_attr( mqd );

  puts( "*** END OF POSIX Message Queue Default Attributes Report ***" );
  exit( 0 );
}
