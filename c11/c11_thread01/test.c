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
static thrd_t   thread2;

void initialize_subsystem_once(void)
{
  printf("*** Subsystem initialization should only happen once\n");
}

void initialize_subsystem(void)
{
  static once_flag initialize_subsystem_only_once = ONCE_FLAG_INIT;

  call_once(&initialize_subsystem_only_once, initialize_subsystem_once);
}

int Thread1_Body(void *arg)
{
  thrd_t *t = (thrd_t *) arg;
  int     rc;
  int     result;

  initialize_subsystem();

  printf("Thread1(0x%08lx)\n", *t);
  assert(thrd_current() == thread1);

  puts("Thread1 - yield" );
  thrd_yield();

  puts("Thread1 - join Thread2" );
  rc = thrd_join(thread2, &result);
  assert(rc == thrd_success);
  printf("Thread1 - thread2 exited with %d\n", result);

  puts("Thread1 - exit" );
  thrd_exit(0);
  return 0;
}

int Thread2_Body(void *arg)
{
  thrd_t *t = (thrd_t *) arg;

  initialize_subsystem();

  printf("Thread2(0x%08lx)\n", *t);
  assert(thrd_current() == thread2);

  puts("Thread2 - exit" );
  thrd_exit(128);
  return 0;
}

int main(int argc, char **argv)
{
  int             rc;
  struct timespec delay = {1, 0};

  puts("*** START OF C11 THREAD EXAMPLE ***");

  initialize_subsystem();

  puts("main - Create Thread1");
  rc = thrd_create(&thread1, Thread1_Body, &thread1);
  assert(rc == thrd_success);

  puts("main - Detach Thread1");
  rc = thrd_detach(thread1);
  assert(rc == thrd_success);

  puts("main - yield to Thread1" );
  thrd_yield();

  puts("main - Create Thread2");
  rc = thrd_create(&thread2, Thread2_Body, &thread2);
  assert(rc == thrd_success);

  puts("Exercise thrd_equal");
  rc = thrd_equal(thread1, thread2);
  assert(rc == 0);

  rc = thrd_equal(thread1, thread1);
  assert(rc != 0);

  puts("main - sleep and let other threads run");
  rc = thrd_sleep(&delay, NULL);
  assert(rc == 0);

  puts("*** END OF C11 THREAD EXAMPLE ***");
  return 0;
}
