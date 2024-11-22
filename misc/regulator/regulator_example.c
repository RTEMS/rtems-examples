/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @defgroup RegulatorExamples Regulator Use Examples
 *
 * @brief User Examples for the Regulator
 *
 * This is a set of user examples for the regulator.
 */

/**
 * @ingroup RegulatorExamples
 *
 * @file
 *
 * @brief Example 1 for Regulator Library
 */

/*
 * Copyright (C) 2022 On-Line Applications Research Corporation (OAR)
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <rtems.h>
#include <rtems/inttypes.h>

#include <rtems/regulator.h>

/**
 * @brief Regulator instance for example visible to entire file.
 */
static rtems_regulator_instance *regulator;

/**
 * @brief Count of Delivered Messages
 */
static int delivered = 1;

/**
 * @ingroup RegulatorExamples
 * @brief Deliver Method Which Prints
 *
 * This deliverer method implementation prints the message with the
 * receipt time. The following behavioral warnings should be noted
 * as this is not necessarily representative of whata real application
 * may do:
 *
 * - The printing may skew the delivery time. 
 * - The message must be in ASCII and NUL terminated.
 */
static bool example_deliverer(
  void     *context,
  void     *message,
  size_t    length
)
{
  (void) context;
  (void) length;

  int              rc;
  struct timespec current;

  rc = clock_gettime(CLOCK_REALTIME, &current);
  assert(rc == 0);

  fprintf(
    stderr,
    "%" PRIdtime_t ":%9ld %s\n",
    current.tv_sec,
    current.tv_nsec,
    (char *)message
  );
  delivered += 1;

  /*
   * When we printed the message, we completed processing it. Because of this,
   * the message buffer can now be released. We have the option of explicitly
   * doing it here and returning false or telling the Delivery Thread to release
   * the buffer by returning true.
   */
  return true;
}

/**
 * @ingroup RegulatorTests
 * @brief Verify rtems_regulator_send and output thread delivers message
 *
 * This example shows that when the regulator is successfully initialized
 * and used as expected, a message sent via rtems_regulator_send() is delivered as
 * expected.
 */
static void example_send_messages(void)
{
  #define MAXIMUM_MESSAGE_LENGTH 32

  rtems_status_code         sc;
  char                      message[MAXIMUM_MESSAGE_LENGTH];
  void                     *buffer;
  size_t                    length;
  int                       msg;
  int                       i;
  int                       burst;

  rtems_regulator_attributes  attributes = {
    .deliverer = example_deliverer,
    .deliverer_context = NULL,
    .maximum_message_size = MAXIMUM_MESSAGE_LENGTH,
    .maximum_messages = 10,
    .delivery_thread_priority = 16,
    .delivery_thread_stack_size = 0,
    .delivery_thread_period = RTEMS_MILLISECONDS_TO_TICKS(1000),
    .maximum_to_dequeue_per_period = 3
  };

  sc = rtems_regulator_create(&attributes, &regulator);
  assert(sc == RTEMS_SUCCESSFUL);
  assert(regulator != NULL);

  /**
   * Send messages as a burst which will need to be smoothly sent at
   * the configured rate.
   */
  msg = 0;
  for (burst=0 ; burst < 2 ; burst++ ) {
    fprintf(
      stderr,
      "Sending Burst %d of 7 messages with 3 delivered per second\n",
      burst
    );
    for (i=1; i <= 7 ; i++, msg++) {
      sc = rtems_regulator_obtain_buffer(regulator, &buffer);
      assert(sc == RTEMS_SUCCESSFUL);
      assert(buffer != NULL);

      length = snprintf(message, MAXIMUM_MESSAGE_LENGTH, "message %d", msg);
      strcpy(buffer, message);

      sc = rtems_regulator_send(regulator, buffer, length);
      assert(sc == RTEMS_SUCCESSFUL);
    }
    sleep(5);
  }

  /* Will not exit above loop */
  #undef MAXIMUM_MESSAGE_LENGTH
}

/* Necessary prototype */
rtems_task test_regulator(rtems_task_argument);

/**
 * @ingroup RegulatorExamples
 * @brief Entry task which invokes the example helper
 */
rtems_task test_regulator(rtems_task_argument arg)
{
  (void) arg;

  puts("*** START OF REGULATOR EXAMPLE 01 ***");

  example_send_messages();

  rtems_regulator_statistics stats;
  rtems_status_code          sc;
  
  sc = rtems_regulator_get_statistics(regulator, &stats);
  assert(sc == RTEMS_SUCCESSFUL);
  fprintf(
    stderr,
    "\n\n*** Regulator Statistics ***\n"
    "obtained/released/delivered: %lld/%lld/%lld\n"
    "periods count/missed: %lld/%lld\n",
    (long long) stats.obtained,
    (long long) stats.released,
    (long long) stats.delivered,
    (long long) stats.period_statistics.count,
    (long long) stats.period_statistics.missed_count
  );

  puts("*** END OF EXAMPLE REGULATOR 01 ***");
  exit(0);
}
