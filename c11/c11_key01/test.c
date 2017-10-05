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
static tss_t    tss1;

typedef struct {
  int thread_specific_value;
} tss_data_t;

void *tss_alloc(int value)
{
  void       *p;
  tss_data_t *k;

  p = malloc(sizeof(tss_data_t));
  k = (tss_data_t  *)p;
  k->thread_specific_value = value; 
  return p;
}

void tss_free(void *p)
{
  free(p);
}

int Thread1_Body(void *arg)
{
  thrd_t     *t = (thrd_t *) arg;
  int         rc;
  tss_data_t *td;

  printf("Thread1(0x%08lx)\n", *t);
  assert(thrd_current() == thread1);

  puts("Thread1 - Assign TSS value for self");
  rc = tss_set(tss1, tss_alloc(1));
  assert(rc == thrd_success);

  puts("Thread1 - yield" );
  thrd_yield();

  td = (tss_data_t *) tss_get(tss1);
  printf("Thread1 - tss value = %d\n", td->thread_specific_value);

  puts("Thread1 - exit" );
  thrd_exit(0);
  return 0;
}

int Thread2_Body(void *arg)
{
  thrd_t     *t = (thrd_t *) arg;
  int         rc;
  tss_data_t *td;

  printf("Thread2(0x%08lx)\n", *t);
  assert(thrd_current() == thread2);

  puts("Thread2 - Assign TSS value for self");
  rc = tss_set(tss1, tss_alloc(2));
  assert(rc == thrd_success);

  puts("Thread2 - yield" );
  thrd_yield();

  td = (tss_data_t *) tss_get(tss1);
  printf("Thread2 - tss value = %d\n", td->thread_specific_value);

  puts("Thread2 - exit" );
  thrd_exit(128);
  return 0;
}

int main(int argc, char **argv)
{
  int              rc;
  struct timespec  delay = {1, 0};
  tss_data_t      *td;

  puts("*** START OF C11 KEY EXAMPLE ***");

  puts("main - Create Thread1");
  rc = thrd_create(&thread1, Thread1_Body, &thread1);
  assert(rc == thrd_success);

  puts("main - Create Thread2");
  rc = thrd_create(&thread2, Thread2_Body, &thread2);
  assert(rc == thrd_success);

  puts("main - Create TSS instance");
  rc = tss_create(&tss1, tss_free);
  assert(rc == thrd_success);

  puts("main - Assign TSS value for self");
  rc = tss_set(tss1, tss_alloc(128));
  assert(rc == thrd_success);

  puts("main - sleep and let threads run");
  rc = thrd_sleep(&delay, NULL);
  assert(rc == 0);

  td = (tss_data_t *) tss_get(tss1);
  printf("main - tss value = %d\n", td->thread_specific_value);

  puts("main - Delete TSS instance");
  tss_delete(tss1);

  puts("*** END OF C11 KEY EXAMPLE ***");
  return 0;
}
