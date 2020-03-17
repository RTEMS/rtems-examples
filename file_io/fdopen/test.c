/*
 *  Simple test program -- demonstrating use of IMFS
 */

#include <bsp.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

rtems_task Init(
  rtems_task_argument ignored
)
{
   int fd;
   FILE *fp;
   char str[256];

   /*
    * Print a startup message
    */ 
   printf(
     "\n\n"
     "%s\n"
     "Here we go!\n",
     _RTEMS_version
   );

   fp = fopen("test", "w");
   fprintf(fp, "Hello world!!!\n");
   fclose(fp);

   fp = fopen("test", "r");
   fgets(str, 200, fp);
   printf("read: %s\n", str);
   fclose(fp);

   fd = open("test", O_WRONLY|O_APPEND);
   printf( "fcntl flags =0x%x\n", fcntl( fd, F_GETFL ) );
   close(fd);

   fd = open("test1", O_CREAT);
   printf( "fcntl flags =0x%x\n", fcntl( fd, F_GETFL ) );
   close(fd);

   fd = open("test", O_RDONLY);
   if (fd == -1) {
     printf("Starting on the wrong foot....\n");
     exit(-1);
   }

   printf( "fcntl flags =0x%x\n", fcntl( fd, F_GETFL ) );

   fp = fdopen(fd, "r");
   if (fp == NULL) {
      printf("Nothing ever goes my way!\n");
      close(fd);
      exit(-1);
   } else {
      printf("Soon, I will be able to take over the world!\n");
      fgets(str, 200, fp);
      printf("%s\n", str);
      fclose(fp);
   }

   exit(0);
}

/* configuration information */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 6

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
