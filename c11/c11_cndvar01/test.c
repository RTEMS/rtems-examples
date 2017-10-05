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
static mtx_t    buffer_mutex;
static cnd_t    buffer_cnd;
static int      buffer_value;

static int buffer_init(void)
{
  int     rc;

  puts("buffer - mtx_init()");
  rc = mtx_init(&buffer_mutex, mtx_timed);
  assert(rc == thrd_success);

  puts("buffer - cnd_init()");
  rc = cnd_init(&buffer_cnd);
  assert(rc == thrd_success);

  buffer_value = -1;
}

void buffer_destroy(void)
{
  puts("buffer - mtx_destroy()" );
  mtx_destroy(&buffer_mutex);

  puts("buffer - cnd_destroy()" );
  cnd_destroy(&buffer_cnd);
}

static int buffer_get(void)
{
  int     rc;
  int     value;

  // puts("buffer - mtx_lock()");
  rc = mtx_lock(&buffer_mutex);
  assert(rc == thrd_success);

    while (buffer_value == -1) {
      // puts("buffer - cnd_wait()");
      rc = cnd_wait(&buffer_cnd, &buffer_mutex);
      assert(rc = thrd_success);
    }

    value = buffer_value;
    buffer_value = -1;
    // puts("buffer - return value");
    

  // puts("buffer - mtx_unlock()");
  rc = mtx_unlock(&buffer_mutex);
  assert(rc == thrd_success);

  return value;
}

static void buffer_put(int value)
{
  int     rc;
  int     old_value;

  // puts("buffer - mtx_lock()");
  rc = mtx_lock(&buffer_mutex);
  assert(rc == thrd_success);

    old_value = buffer_value;
    buffer_value = value;

    if (old_value == -1) {
      // puts("buffer - cnd_signal()");
      rc = cnd_signal(&buffer_cnd);
      assert(rc = thrd_success);
    }

  // puts("buffer - mtx_unlock()");
  rc = mtx_unlock(&buffer_mutex);
  assert(rc == thrd_success);

}

int Thread1_Body(void *arg)
{
  int     rc;
  int     value;

  (void) arg;

  puts("Thread1 - Loop for values");
  while (1) {
    value = buffer_get();
    printf("Thread1 value = %d\n", value);
  }

  return 0;
}

int main(int argc, char **argv)
{
  int             rc;
  int             value;
  struct timespec delay = {1, 0};

  puts("*** START OF C11 CONDITION VARIABLE EXAMPLE ***");

  puts("main - Create Thread1");
  rc = thrd_create(&thread1, Thread1_Body, &thread1);
  assert(rc == thrd_success);

  buffer_init();

  puts("main - yield to Thread1" );
  thrd_yield();

  for (value=1 ; value <= 3 ; value++) {
    printf("main - put value = %d\n", value * 10);
    buffer_put(value * 10);

    puts("main - sleep to let Thread1 get the value");
    rc = thrd_sleep(&delay, NULL);
    assert(rc == 0);
  }

  buffer_destroy();

  puts("*** END OF C11 CONDITION VARIABLE EXAMPLE ***");
  return 0;
}
