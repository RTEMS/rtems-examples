/* SPDX-License-Identifier: BSD-2-Clause */

/*
 *  Starting point for this example.  Can be compiled on RTEMS or Linux.
 */

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

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

extern void doTest(void);

#if defined(__rtems__)
#include <bsp.h>

extern "C" {
  rtems_task Init(
    rtems_task_argument arg
  );
};

rtems_task Init(
  rtems_task_argument arg
)
#else
int main(
  int    argc,
  char **argv
)
#endif
{
#if !defined(BSP_SMALL_MEMORY)
  doTest();
#endif
  exit( 0 );
}

#if defined(__rtems__)
/* RTEMS configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MICROSECONDS_PER_TICK RTEMS_MILLISECONDS_TO_MICROSECONDS(1)

#define CONFIGURE_INIT_TASK_ATTRIBUTES RTEMS_FLOATING_POINT
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#if defined(BSP_SMALL_MEMORY)
  #define CONFIGURE_MAXIMUM_TASKS 1
#else
  #define CONFIGURE_MAXIMUM_TASKS 102
  #define CONFIGURE_MAXIMUM_PERIODS  100
  #define CONFIGURE_MAXIMUM_POSIX_THREADS 2
  #define CONFIGURE_EXTRA_TASK_STACKS (32 * 1024)

  #define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
  #define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 20

  #define CONFIGURE_STACK_CHECKER_ENABLED
#endif

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
#endif

/* end of file */
