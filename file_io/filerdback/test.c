/*
 *  Simple test program to write and read a file back.
 */

#include <bsp.h>

#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>

char *bufr = "Happy days are here again.  Happy days are here again.1Happy "
"days are here again.2Happy days are here again.3Happy days are here again."
"4Happy days are here again.5Happy days are here again.6Happy days are here "
"again.7Happy days are here again.";

rtems_task Init(
  rtems_task_argument ignored
)
{
   int fd;
   int i, n, total;
   char *bufr2;

   printf( "BUFSIZ = %d\n", BUFSIZ );
   bufr2 = (char *)malloc(BUFSIZ);
   fd = creat("tester", S_IRWXU | S_IRWXG | S_IRWXO);
   for (i=0; i<10; i++) {
      n=write(fd, bufr, strlen(bufr));
      printf("Wrote %d\n", n);
   }
   close(fd);

   total = 0;
   fd = open("tester", O_RDONLY);
   while ((n=read(fd,bufr2,60)) > 0) {
      bufr2[n-1] = '\0';
      total += n;
      printf("Read=%d, total=%d : %s\n", n, total, bufr2);
   }
   free(bufr2);
   close(fd);
   exit(0);
}

/* configuration information */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_TASKS 1
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 4

#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* end of file */
