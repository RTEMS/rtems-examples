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

  printf("<child>: Hello World! task with max priority \n");
  clock_gettime( CLOCK_REALTIME, &now );

  printf("\nnow tv_sec = %lld, tv_nsec = %ld\n", now.tv_sec, now.tv_nsec);

  timeout.tv_sec  = now.tv_sec + 3;
  timeout.tv_nsec = now.tv_nsec;

  printf("timeout tv_sec = %lld, tv_nsec = %ld\n", timeout.tv_sec, timeout.tv_nsec);
  printf("The task is coming to enter in a timed wait\n");
  pthread_cond_timedwait(&cond, &mutex, &timeout);
  printf("The task is coming out from the timed wait \n");
  return NULL;
}

void * print_hello_a(void * arg)
{
  printf(" <child>: Hello World! Task with lowest priority ");
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

  printf("<main> Enter in the main \n");

  printf("Creating first task \n");
  param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  pthread_attr_setschedparam(&attr, &param);
  if ( pthread_create( &child1, &attr, print_hello, NULL) || 
       pthread_setschedparam(child1, SCHED_FIFO, &param) ) {
    printf(
      "Thread cannot be created or you have not enough privileges \n"
      "    to set priority!!!!\n");
    exit(1);
  }

  printf("First Task created \n");
  printf("Creating second task \n");
  param.sched_priority = sched_get_priority_max(SCHED_FIFO) - 1;
  pthread_attr_setschedparam(&attr, &param);
  if ( pthread_create( &child2, &attr, print_hello_a, NULL) || 
       pthread_setschedparam(child2, SCHED_FIFO, &param) ) {
    printf(
      "Thread cannot be created or you have not enough privileges \n"
      "    to set priority!!!!\n");
    exit(1);
  }
  printf("Second task created \n");

  printf("<main> Out of the main\n");
  pthread_join( child1, NULL );
  pthread_join( child2, NULL );

  exit(0);
}

#if defined(__rtems__)
#include <bsp.h>

static void *POSIX_Init()
{
  return (void *)main(0, NULL);
}

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS              10
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
#endif



