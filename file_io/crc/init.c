/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */



#include <rtems.h>
#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>

#include "fs-root-tar.h"
#include <rtems/untar.h>

/* sha-specific includes */
#include <string.h>
#include <time.h>
#include "crc.h"
#include <errno.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
  char in_file[20] = "/image.img";
  DWORD crc;
  long charcnt;

  puts( "\n\n*** untar/imfs/crc example ***" );

  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory((char*) fs_root_tar, fs_root_tar_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  crc32file(in_file,&crc, &charcnt);
  printf("%08lX %7ld %s\n", crc, charcnt, in_file);

  printf( "*** end of demonstration ***\n" );
  exit( 0 );
}

/* configuration */
/* drivers */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

/* filesystem */
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 40
#define CONFIGURE_IMFS_MEMFILE_BYTES_PER_BLOCK 512

/* tasks */
#define CONFIGURE_MAXIMUM_TASKS             4
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_EXTRA_TASK_STACKS         (3 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
