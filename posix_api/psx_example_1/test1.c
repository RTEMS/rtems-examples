#include <sched.h>
#include <bsp.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void * print_hello(void * arg)
{
  sleep(1);
  printf("<child>: Hello World!\n");
  return NULL;
}

void *POSIX_Init()
{
  pthread_t child1;
  pthread_t child2;
  int status;

  status = pthread_create( &child1, NULL, print_hello, NULL );
  if ( status ) perror("Error on create child 1");

  status = pthread_create( &child2, NULL, print_hello, NULL );
  if ( status ) perror("Error on create child 1");

  printf("<main>: Wait for child2 thread...\n");

  status = pthread_join( child1, NULL );
  if ( status ) perror ("Error on join");
  printf("<main>: Successfully joined with child1\n" );
  exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS              10
#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
