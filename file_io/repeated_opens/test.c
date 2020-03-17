/* Open/close test */

#include <rtems.h>
#include <rtems/clockdrv.h>
#include <rtems/console.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

rtems_task Init(rtems_task_argument a)
{
    int         i;
    int         fd;
    char        msg[16];
    int         len;
    int         l;

    printf("Open/close test (%d)\n",(int)a);

    strcpy(msg,"OK!\n");
    len = strlen(msg);

    for(i = 0; i < 10000; i++){
        fd = open("/tempfile",O_RDWR|O_CREAT, 0666);
        if(fd < 0){
            printf("open failed on %dth try (%s).\n",i, strerror(errno));
            continue;
        }
        l = write(fd,msg,len);
        if(l != len){
            printf("write failed on %dth try. (%d)\n",i,l);
            break;
        }
        close(fd);
        if ( i && (i%100 == 0)) printf("pass %d\n", i);
        
    }
    printf("Done.\n");
    exit(0);
}

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_MAXIMUM_TASKS            1
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS 4
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

