/**
 *  @brief C11 Threads Example
 */

/*
 * This file's license is 2-clause BSD as in this distribution's
 * LICENSE.2 file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <assert.h>

static thrd_t   thread1;
static mtx_t    mutex1;

int Thread1_Body(void *arg)
{
  int     rc;
  struct timespec delay;

  (void) arg;

  puts("Thread1 - mtx_trylock(mutex1) -> thrd_busy");
  rc = mtx_trylock(&mutex1);
  assert(rc == thrd_busy);

  puts("Thread1 - mtx_timedlock(mutex1, 2 second) - thrd_timeout");
  delay.tv_sec = time(NULL) + 1;
  delay.tv_nsec = 0;
  rc = mtx_timedlock(&mutex1, &delay);
  assert(rc == thrd_timedout);

  puts("Thread1 - mtx_lock(mutex1)");
  rc = mtx_lock(&mutex1);
  assert(rc == thrd_success);

  puts("Thread1 - yield" );
  thrd_yield();

  puts("Thread1 - exit" );
  thrd_exit(0);
  return 0;
}

int main(int argc, char **argv)
{
  int             rc;
  struct timespec delay = {5, 0};

  puts("*** START OF C11 MUTEX EXAMPLE ***");

  puts("main - Create Thread1");
  rc = thrd_create(&thread1, Thread1_Body, &thread1);
  assert(rc == thrd_success);

  puts("main - mtx_init(mutex1)");
  rc = mtx_init(&mutex1, mtx_timed);
  assert(rc == thrd_success);

  puts("main - mtx_lock(mutex1)");
  rc = mtx_lock(&mutex1);
  assert(rc == thrd_success);

  puts("main - yield to Thread1" );
  thrd_yield();

  puts("main - sleep to let Thread2's delay complete while locked");
  rc = thrd_sleep(&delay, NULL);
  assert(rc == 0);

  puts("main - mtx_unlock(mutex1)" );
  rc = mtx_unlock(&mutex1);
  assert(rc == thrd_success);

  puts("main - yield to Thread1" );
  thrd_yield();

  puts("main - mtx_destroy(mutex1)" );
  mtx_destroy(&mutex1);

  puts("*** END OF C11 MUTEX EXAMPLE ***");
  return 0;
}
