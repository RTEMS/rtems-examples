/*
 *  Program to test restarting a POSIX thread
 */

/*
 * Copyright 2021 Joel Sherrill (joel@rtems.org)
 *
 * This file's license is 2-clause BSD as in this distribution's LICENSE.2 file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>
#include <assert.h>

#include <rtems.h>
#include <rtems/bspIo.h>

#define HEALTH_EVENT 1

pthread_t Health_thread_id;
pthread_t Faulting_thread_id;

void print_priority(const char *name)
{
  rtems_status_code    status;
  rtems_task_priority  current;

  status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &current);
  assert(status == RTEMS_SUCCESSFUL);
  
  printk("%s priority --> %d\n", name, current);
}

rtems_task Health_Thread(rtems_task_argument arg)
{
  rtems_status_code status;
  rtems_event_set   events;

  (void) arg;

  printk("Health Thread entered\n");
  print_priority("Health thread"); 

  while (1) {
    status = rtems_event_receive(
      HEALTH_EVENT,
      RTEMS_DEFAULT_OPTIONS,  /* block forever */
      RTEMS_NO_TIMEOUT,
      &events
    );
    assert(status == RTEMS_SUCCESSFUL);

    printk("Health Thread restarting Faulting thread\n");
    status = rtems_task_restart(Faulting_thread_id, 1);
    assert(status == RTEMS_SUCCESSFUL);
  }

  printk("Health Thread exiting\n");
}

void *Faulting_thread(void *arg)
{
  rtems_status_code status;

  printk("Faulting thread entered: %p\n", arg);
  print_priority("Faulting thread"); 

#if USE_PTHREADS
  int rc;
  rc = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  assert(rc ==  0);
#endif
  if (arg == (void *) 1) {
    printk( "Faulting thread Successfully restarted\n");
    puts( "*** END OF POSIX Thread Restart ***" );
    exit( 0 );
  } else {
    printk( "Faulting thread sending health event\n");
    status = rtems_event_send(Health_thread_id, HEALTH_EVENT);
    assert(status == RTEMS_SUCCESSFUL);

    printk( "Faulting thread should not be here\n");
    /* Do we get restarted or return to here? */
    while (1) { }
  };
  return NULL;
}

void start_health_thread(void)
{
  rtems_status_code     status;

  /* Does not matter is Health thread is higher or lower priority */
  status = rtems_task_create(
    rtems_build_name('H','L','T','H'),
    0,
    16 * 1024,
    RTEMS_DEFAULT_MODES,
    RTEMS_SYSTEM_TASK,
    &Health_thread_id
  );
  assert(status == RTEMS_SUCCESSFUL);

  status = rtems_task_start(Health_thread_id, Health_Thread, 0);
  assert(status == RTEMS_SUCCESSFUL);
}

void start_faulting_thread(void)
{
  rtems_status_code     status;

#if USE_PTHREADS
  int                  rc;
  rtems_task_priority  old;
  
  printk("Main: pthread_create faulting ... \n");
  rc = pthread_create(&Faulting_thread_id, NULL, Faulting_thread, NULL);
  assert(rc == 0);

  status = rtems_task_set_priority(Faulting_thread_id, 2, &old);
  assert(status == RTEMS_SUCCESSFUL);

#else
  printk("Main: rtems_task_create faulting ... \n");

  status = rtems_task_create(
    rtems_build_name('F','A','L','T'),
    1,
    16 * 1024,
    RTEMS_DEFAULT_MODES,
    RTEMS_DEFAULT_ATTRIBUTES,
    &Faulting_thread_id
  );
  assert(status == RTEMS_SUCCESSFUL);

  status = rtems_task_start(Faulting_thread_id, (rtems_task_entry)Faulting_thread, 0);
  assert(status == RTEMS_SUCCESSFUL);

#endif
}

int main()
{
  puts( "*** POSIX Thread Restart ***" );

  print_priority("main");

  start_health_thread();

  start_faulting_thread();

  sleep(5);

  printk("main() - Should not get here");
  assert(0);
}
