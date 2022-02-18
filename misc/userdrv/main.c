/**
 *  @file
 *
 *  @brief Program to Exercise Example User Driver
 */

/*
 * Copyright (C) 2022 OAR Corporation
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "userdrv.h"    /* for ioctl numbers */

int main(
  int    argc,
  char **argv
)
{
  int      drv;
  int      rc;
  ssize_t  bytes;

  printf( "\n\n*** USER DRIVER TEST ***\n" );

  /* Exercise open() */
  printf( "open(userdrv)\n" );
  drv = open("/dev/userdrv", O_RDWR);
  assert(drv != -1);

  /* Exercise write() */
  const char string_to_write[] = "Application data to write with driver.";
  size_t     to_write = strlen(string_to_write);

  printf( "\nwrite(%d, %s)\n", to_write, string_to_write );
  bytes = write(drv, string_to_write, to_write); 
  printf("  write() returned bytes written=%d\n", bytes);
  assert(bytes == to_write);

// TBD write should be returning number written

  /* Exercise read() */
  char   inbuf[80];
  size_t bufsize = 80;

  printf( "\nread(%d)\n", bufsize);
  bytes = read(drv, inbuf, bufsize);
  printf("  read() returned bytes read=%d\n", bytes);
  printf("  %d bytes: %s\n", bytes, inbuf);

// TBD write should be returning number written

  /* Exercise ioctl() */
  const char *ioctl_string = "ioctl buffer string";
  printf( "\nioctl(%s)\n", ioctl_string);
  rc = ioctl(drv, USERDRV_IOCTL_DO_SOMETHING, ioctl_string);
  assert(rc == 0);
  printf( "  ioctl() returned %d\n", rc);
/* TBD */

  /* Exercise fclose() */
  printf( "\nclose(userdrv)\n" );
  rc = close(drv);
  assert(rc == 0);
  printf( "  close() returned %d\n", rc);

  printf( "*** END OF USER DRIVER TEST ***\n" );
  exit( 0 );
}
/* end of file */
