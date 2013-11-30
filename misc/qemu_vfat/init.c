/*
 *  COPYRIGHT (c) 1989-2010.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#define CONFIGURE_INIT
#include "system.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <rtems.h>
#include <fcntl.h>
#include <rtems/error.h>
#include <rtems/dosfs.h>
#include <ctype.h>
#include <rtems/bdpart.h>
#include <rtems/libcsupport.h>
#include <rtems/fsmount.h>

/*
 * Table of FAT file systems that will be mounted
 * with the "rtems_fsmount" function.
 * See cpukit/libmisc/fsmount for definition of fields
 */
#define MOUNT_POINT "/mnt/test"
fstab_t fs_table[] = {
  {
    "/dev/hda1", MOUNT_POINT,
    "dosfs", RTEMS_FILESYSTEM_READ_WRITE,
    FSMOUNT_MNT_OK | FSMOUNT_MNTPNT_CRTERR | FSMOUNT_MNT_FAILED,
    0
  },
};

#ifndef USE_SHELL
void start_test(void);
#endif

#ifdef USE_SHELL
#include <rtems/shell.h>

static void writeFile(
  const char *name,
  mode_t      mode,
  const char *contents
)
{
  int sc;
  sc = setuid(0);
  if ( sc ) {
    printf( "setuid failed: %s: %s\n", name, strerror(errno) );
  }

  rtems_shell_write_file( name, contents );

  sc = chmod ( name, mode );
  if ( sc ) {
    printf( "chmod %s: %s\n", name, strerror(errno) );
  }
}

#define writeScript( _name, _contents ) \
        writeFile( _name, 0777, _contents )

static void fileio_start_shell(void)
{
  int sc;

  sc = mkdir("/scripts", 0777);
  if ( sc ) {
    printf( "mkdir /scripts: %s:\n", strerror(errno) );
  }

  sc = mkdir("/etc", 0777);
  if ( sc ) {
    printf( "mkdir /etc: %s:\n", strerror(errno) );
  }

  fprintf(
    stderr, 
    "Creating /etc/passwd and group with three useable accounts\n"
    "root/pwd , test/pwd, rtems/NO PASSWORD"
  );

  writeFile(
    "/etc/passwd",
    0644,
    "root:7QR4o148UPtb.:0:0:root::/:/bin/sh\n"
    "rtems:*:1:1:RTEMS Application::/:/bin/sh\n"
    "test:8Yy.AaxynxbLI:2:2:test account::/:/bin/sh\n"
    "tty:!:3:3:tty owner::/:/bin/false\n"
  );
  writeFile(
    "/etc/group",
    0644,
    "root:x:0:root\n"
    "rtems:x:1:rtems\n"
    "test:x:2:test\n"
    "tty:x:3:tty\n"
  );

  rtems_shell_init(
    "SHLL",                          /* task_name */
    RTEMS_MINIMUM_STACK_SIZE * 4,    /* task_stacksize */
    100,                             /* task_priority */
    "/dev/console",                  /* devname */
    false,                           /* forever */
    true,                            /* wait */
    rtems_shell_login_check          /* login */
  );
}
#endif /* USE_SHELL */

/*
 * RTEMS Startup Task
 */
rtems_task
Init (rtems_task_argument ignored)
{
  rtems_status_code rc;

  puts( "\n\n*** QEMU VFAT AND SHELL TEST ***" );

  rc = rtems_bdpart_register_from_disk("/dev/hda");
  if ( rc != RTEMS_SUCCESSFUL ) {
    fprintf( stderr, "Unable to initialize partition table from /dev/hda\n" );
    exit(1);
  }

  rc = rtems_fsmount(
    fs_table,
    sizeof(fs_table)/sizeof(fs_table[0]),
    NULL
  );
  if ( rc != RTEMS_SUCCESSFUL ) {
    fprintf( stderr, "Unable to mount /dev/hda1\n" );
    exit(1);
  }

#if defined(USE_SHELL)
  fileio_start_shell ();
#endif
#if defined(USE_START_TEST)
  chdir( MOUNT_POINT );
  start_test ();
#endif
  puts( "*** END OF QEMU VFAT AND SHELL TEST ***" );
  exit(0);
}

#if defined(USE_SHELL)

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS

#include <rtems/shellconfig.h>
#endif
