/*
 *  Simple test program -- simplified version of sample test hello.
 */

#include <bsp.h>

#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include <rtems/umon.h>

/* this code is lifted from the umon demo application */
int umon_demo(int argc, char **argv)
{
  int    i, tfd;
  char  line[80], *ab, *filename;

  /* If argument count is greater than one, then dump out the
   * set of CLI arguments...
   */
  if (argc > 1) {
    mon_printf("Argument list...\n");
    for(i=0;i<argc;i++) {
      mon_printf("  arg[%d]: %s\n",i,argv[i]);
      if (strcmp(argv[i],"match") == 0)
        mon_printf("got a match!\n");
    }
  }

  /* Using the content of the shell variable APPRAMBASE, dump the
   * memory starting at that location...
   */
  ab = mon_getenv("APPRAMBASE");
  if (ab) {
    char *addr = (char *)strtoul(ab,0,0);

    mon_printf("Dumping memory at 0x%lx...\n",addr);
    mon_printmem(addr,128,1);
  }

  filename = "csbboot";

  /* If the 'monrc' file exists, the assume it is ASCII and dump it
   * line by line...
   */
  if (mon_tfsstat(filename)) {
    mon_printf("Dumping content of '%s'...\n",filename);

    tfd = mon_tfsopen(filename,TFS_RDONLY,0);
    if (tfd >= 0) {
      while(mon_tfsgetline(tfd,line,sizeof(line)))
        mon_printf("%s",line);
      mon_tfsclose(tfd,0);
    }
    else {
      mon_printf("TFS error: %s\n",
        (char *)mon_tfsctrl(TFS_ERRMSG,tfd,0));
    }
  }
  else {
    mon_printf("File '%s' not found\n",filename);
  }
  return 0;
}

void catFile(const char *name)
{
  int   i;
  FILE *fp;
  char  line[128];

  if (!name)
    return;

  printf( "====> Start of file %s\n", name );
  /* Attempt to open and print a file through the
   * the TFS based FS...
   */
  fp = fopen(name,"r");
  if (!fp) {
    printf("fopen() failed\n");
    perror("fopen()");
    return;
  } 

  for (i=0 ; fgets(line,sizeof(line),fp) ; i++)
    printf("LINE %d: %s",i, line);

  printf( "====> End of file %s\n", name );
  fclose(fp);
}

void tfsFsDemo(void)
{
  char *name;

  /* Initialize the TFS based file system...
   */
  puts( "Mounting RTEMS TFS filesystem" );
  if (rtems_initialize_tfs_filesystem( "/JOEL/" ) != 0) {
    puts("tfs init failed");
    return;
  }

  /* Use uMon's environment to store the name of the file.
   * If the shell variable TFSFS_FILENAME is present, then
   * assume it contains the name of the file to open and
   * print to the console...
   * For TFS to run as a file system under RTEMS, all filenames
   * that are associated with tfs are preceded with "/TFS/".
   */
  name = mon_getenv("TFSFS_FILENAME");
  if (!name)
    puts("Shell variable TFSFS_FILENAME not available");
  else
    catFile( name );
  catFile( "/JOEL/csbboot" );
  catFile( "/JOEL/boot" );
}


rtems_task Init(
  rtems_task_argument ignored
)
{
  puts( "\n\n*** MicroMonitor Demonstration ***" );

  puts( "calling monConnect ..." );
  rtems_umon_connect();

  umon_demo(0, NULL);
  tfsFsDemo();

  puts( "*** MicroMonitor Demonstration ***" );
  exit( 0 );
}

/* configuration information */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1
#define CONFIGURE_MAXIMUM_SEMAPHORES 5
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 4
#define CONFIGURE_MICROSECONDS_PER_TICK 5000
#define CONFIGURE_INIT_TASK_STACK_SIZE  \
  (CONFIGURE_MINIMUM_TASK_STACK_SIZE * 10)

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* end of file */
