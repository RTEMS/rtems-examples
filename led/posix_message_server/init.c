/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * COPYRIGHT (c) 2026.
 * On-Line Applications Research Corporation (OAR).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>

#include "../led.h"

#define CHECK( _msg, _rc, _errno ) \
  do { \
    if ( (_rc) != 0 ) { \
      printf( "Server: %s %d %s\n", _msg, _rc, strerror(_errno) ); \
    } \
    assert( _rc == 0 ); \
  } while (0)

#define QUEUE_NAME "LED"

/*
 * Initialize a message queue structure for the desired attributes.
 */
struct mq_attr Attr = {
  .mq_flags = 0,
  .mq_maxmsg = 10,
  .mq_msgsize = 4,
  .mq_curmsgs = 0
};

static void *Message_Server(
  void *unused
)
{
  (void) unused;

  mqd_t        rx_queue;
  uint32_t     count;
  unsigned int prio;
  int          rc;

  /*
   * Open the queue for reading
   *
   * Note that it does not include the permissions or attributes. Those
   * are only used when creating the message queue. The following is an
   * example of how it should not be done.
   *
   * rx_queue = mq_open(QUEUE_NAME, O_RDONLY, 0644, &Attr);
   */
  rx_queue = mq_open(QUEUE_NAME, O_RDONLY);
  if (rx_queue == (mqd_t)-1) {
      perror("mq_open");
      exit(0);
  }

  for ( ; ; ) {
    count = 0xFFFFFFFF;
    rc = mq_receive(rx_queue, (char *)&count, sizeof(count), &prio );
    if ( rc != sizeof(count) ) {
      printf( "mq_receive %d expected %d\n", rc, sizeof(count) );
      exit(0);
    }

    if ( (count % 2) == 0 ) {
      LED_OFF();
    } else {
      LED_ON();
    }
  }
}

static void *POSIX_Init(
  void *unused
)
{
  (void) unused;

  mqd_t      tx_queue;
  uint32_t   count;
  pthread_t  thread_id;
  int        rc;

  puts( "\n\n*** LED BLINKER -- POSIX message receive server ***" );

  LED_INIT();

  tx_queue = mq_open("LED", O_RDWR | O_CREAT, 0666, &Attr);
  assert( tx_queue != (mqd_t) -1 );

  rc = pthread_create( &thread_id, NULL, Message_Server, NULL);
  assert( rc == 0 );

  for (count=0; ; count++) {

    rc = mq_send( tx_queue, (const char *)&count, sizeof(uint32_t), 1 );
    assert( rc == 0 );

    sleep( 1 );
  }

  return NULL;
}

#include <bsp.h>

/**************** START OF CONFIGURATION INFORMATION ****************/

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_MESSAGE_QUEUES 1
#define CONFIGURE_MESSAGE_BUFFER_MEMORY \
           CONFIGURE_MESSAGE_BUFFERS_FOR_QUEUE(1, sizeof(uint32_t))

#define CONFIGURE_POSIX_INIT_THREAD_TABLE
#define CONFIGURE_MAXIMUM_POSIX_THREADS 20

#define CONFIGURED_UNLIMITED_OBJECTS
#define CONFIGURED_UNIFIED_WORK_AREAS

#define CONFIGURE_MAXIMUM_PROCESSORS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/****************  END OF CONFIGURATION INFORMATION  ****************/
