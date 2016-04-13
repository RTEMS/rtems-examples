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

#include "FilesystemImage.h"


/**
 * The RAM Disk configuration.
 */
rtems_ramdisk_config rtems_ramdisk_configuration[] =
{
  {
    block_size: 256,
    block_num:  1024,
    location:   NULL
  }
};

/**
 * The number of RAM Disk configurations.
 */
size_t rtems_ramdisk_configuration_size = 1;

/**
 * Create the RAM Disk Driver entry.
 */
rtems_driver_address_table rtems_ramdisk_io_ops = {
  initialization_entry: ramdisk_initialize,
  open_entry:           rtems_blkdev_generic_open,
  close_entry:          rtems_blkdev_generic_close,
  read_entry:           rtems_blkdev_generic_read,
  write_entry:          rtems_blkdev_generic_write,
  control_entry:        rtems_blkdev_generic_ioctl
};

#define RTEMS_DRIVER_AUTO_MAJOR (0)

int setup_ramdisk (const char* mntpath)
{
  rtems_device_major_number major;
  rtems_status_code         sc;

  /*
   * Register the RAM Disk driver.
   */
  printf ("Register RAM Disk Driver: ");
  sc = rtems_io_register_driver (RTEMS_DRIVER_AUTO_MAJOR,
                                 &rtems_ramdisk_io_ops,
                                 &major);
  if (sc != RTEMS_SUCCESSFUL) {
    printf ("error: ramdisk driver not initialised: %s\n",
            rtems_status_text (sc));
    return 1;
  }

  printf ("successful\n");
  return 0;
}

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
  if(Untar_FromMemory((char*) FilesystemImage, FilesystemImage_size) != 0) {
    printf("Can't unpack tar filesystem\n");
    exit(1);
  }

  ret = setup_ramdisk ("/mnt/ramdisk");
  if (ret)
    exit (ret);

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

/* filesystem */
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS   40
#define CONFIGURE_IMFS_MEMFILE_BYTES_PER_BLOCK    512
#define CONFIGURE_MAXIMUM_DRIVERS                  10

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
