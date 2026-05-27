/* SPDX-License-Identifier: BSD-2-Clause */

/*
 *  Portable POSIX ticker example.
 */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
   const char *name;
   unsigned int delay;
} thread_info_t;

thread_info_t Thread_Info[] = {
  { "IGN", 0 },
  { "TA1", 5 },
  { "TA2", 10 },
  { "TA3", 15 },
  { "",    0 }
};

static void print_time(
  thread_info_t *t,
  struct timespec *now
)
{
  /* print time as TOD */
  struct tm split, *tmp;

  tmp = gmtime_r(&now->tv_sec, &split);
  assert(tmp != NULL);

  printf(
    "%s - %d/%d/%d %d:%d:%d\n",
    t->name,
    split.tm_mon,
    split.tm_mday,
    split.tm_year + 1900,
    split.tm_hour,
    split.tm_min,
    split.tm_sec
  );
}

void *User_Thread(void *arg)
{
  thread_info_t *t = (thread_info_t *)arg;
  unsigned int seconds = t->delay;

  while (1) {
    int rc;
    struct timespec now;

    rc = clock_gettime( CLOCK_REALTIME, &now );
    assert( rc == 0 );

    print_time( t, &now );

    sleep( seconds );
  }

  return NULL;
}

int main(
  int argc,
  char **argv
)
{
  (void) argc;
  (void) argv;

  int rc;
  int i;
  pthread_t thread_id[4];

  printf( "\n\n*** POSIX Ticker Example ***\n" );

  /* Create three POSIX threads. One sleeps 5, one 10, and one 15 seconds */
  for (i=1 ; i<4 ; i++) {
    pthread_attr_t attr;

    rc = pthread_attr_init( &attr );
    assert(rc == 0);

    rc = pthread_create( &thread_id[i], &attr, User_Thread, &Thread_Info[i]);
    assert(rc == 0);

    rc = pthread_attr_destroy( &attr );
    assert(rc == 0);

  }

  /* Shuts down after 32 seconds */
  sleep(32);

  printf( "*** END OF POSIX Ticker Example ***\n" );

  /*
   * Can return or call exit() directly.
   */
  return 0;
}
