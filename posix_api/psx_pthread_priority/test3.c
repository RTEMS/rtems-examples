/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * This is an example of using setting pthread priority. There are two threads
 * which will execute in priority order and then exit. The initialization
 * thread joins with both before exiting.
 */

#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void * print_hello(void * arg)
{
  struct timespec now;
  struct timespec timeout;

  puts("<child>: Hello World! task with max priority");
  clock_gettime( CLOCK_REALTIME, &now );

  printf("\nnow tv_sec = %lld, tv_nsec = %ld\n",
    (long long) now.tv_sec, now.tv_nsec);

  timeout.tv_sec  = now.tv_sec + 3;
  timeout.tv_nsec = now.tv_nsec;

  printf("timeout tv_sec = %lld, tv_nsec = %ld\n",
    (long long) timeout.tv_sec, timeout.tv_nsec);
  puts("The task is coming to enter in a timed wait");
  pthread_cond_timedwait(&cond, &mutex, &timeout);
  puts("The task is coming out from the timed wait");
  return NULL;
}

void * print_hello_a(void * arg)
{
  puts("<child>: Task with lowest priority");
  return NULL;
}

int main(int argc, char **argv)
{
  pthread_attr_t      attr;
  pthread_t           child1;
  pthread_t           child2;
  struct sched_param  param;

  (void) argc;
  (void) argv;

  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  pthread_mutex_init( &mutex, NULL );
  pthread_cond_init( &cond, NULL );

  printf("*** POSIX Set Thread Priority Example ***\n");

  printf("Creating first task \n");
  param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  pthread_attr_setschedparam(&attr, &param);
  if ( pthread_create( &child1, &attr, print_hello, NULL) || 
       pthread_setschedparam(child1, SCHED_FIFO, &param) ) {
    printf(
      "Thread cannot be created or you do not have privileges\n"
      "    to set priority!!!!\n");
    exit(1);
  }

  puts("Creating second task");
  param.sched_priority = sched_get_priority_max(SCHED_FIFO) - 1;
  pthread_attr_setschedparam(&attr, &param);
  if ( pthread_create( &child2, &attr, print_hello_a, NULL) || 
       pthread_setschedparam(child2, SCHED_FIFO, &param) ) {
    puts(
      "Thread cannot be created or you have not enough privileges \n"
      "    to set priority!!!!");
    exit(1);
  }

  puts("<main> Wait for threads to exit");
  pthread_join( child1, NULL );
  pthread_join( child2, NULL );

  puts("*** END OF POSIX Thread Priority Exapmle ***");
  exit(0);
}

#if defined(__rtems__)
#include <bsp.h>

static void *POSIX_Init(void *arg)
{
  (void) arg;

  (void) main(0, NULL);
  return NULL;
}

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS              10
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
#endif



