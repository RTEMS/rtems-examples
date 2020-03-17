/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */


#include <bsp.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <rtems/ramdisk.h>
#include <rtems/error.h>
#include <rtems/untar.h>
#include <rtems/shell.h>

#include "fs-root-tar.h"


/**
 * The RAM Disk configuration.
 */
rtems_ramdisk_config rtems_ramdisk_configuration[] =
{
  {
    .block_size = 512,
    .block_num = 1024
  }
};

/**
 * The number of RAM Disk configurations.
 */
size_t rtems_ramdisk_configuration_size = 1;

/**
 * Run the /shell-init script.
 */
void shell_init_script(void)
{
  rtems_status_code sc;
  printf("Running /shell-init....\n\n");
  sc = rtems_shell_script("fstst", 60 * 1024, 160, "/shell-init", "stdout",
                           0, 1, 1);
  if (sc != RTEMS_SUCCESSFUL)
    printf("error: running shell script: %s (%d)\n",
             rtems_status_text (sc), sc);
}

/**
 * Start the RTEMS Shell.
 */
void shell_start ()
{
  rtems_status_code sc;
  printf ("Starting shell....\n\n");
  sc = rtems_shell_init ("fstst", 60 * 1024, 150, "/dev/console", 0, 1, NULL);
  if (sc != RTEMS_SUCCESSFUL)
    printf ("error: starting shell: %s (%d)\n", rtems_status_text (sc), sc);
}



rtems_task Init(
  rtems_task_argument ignored
)
{
  int ret;

  puts( "\n\n*** ramdisk/fat example ***" );

  printf("Unpacking tar filesystem\nThis may take awhile...\n");
  if(Untar_FromMemory((char*) fs_root_tar, fs_root_tar_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  shell_init_script();
  shell_start();

  printf( "*** end of demonstration ***\n" );
  exit( 0 );
}


/* configuration */

#define CONFIGURE_MAXIMUM_POSIX_KEYS             16
#define CONFIGURE_MAXIMUM_POSIX_KEY_VALUE_PAIRS  16

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS

#include <rtems/shellconfig.h>

/* drivers */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_EXTRA_DRIVERS RAMDISK_DRIVER_TABLE_ENTRY

/* filesystem */
#define CONFIGURE_FILESYSTEM_DOSFS
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS   40
#define CONFIGURE_IMFS_MEMFILE_BYTES_PER_BLOCK    512

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_SWAPOUT_TASK_PRIORITY            10

#define CONFIGURE_MAXIMUM_TASKS                    rtems_resource_unlimited (10)
#define CONFIGURE_MAXIMUM_SEMAPHORES               rtems_resource_unlimited (10)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES           rtems_resource_unlimited (5)
#define CONFIGURE_MAXIMUM_PARTITIONS               rtems_resource_unlimited (2)
#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
