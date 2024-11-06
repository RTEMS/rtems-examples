/* SPDX-License-Identifier: BSD-2-Clause */

/*
 *  Portable POSIX hello world example.
 *  This provides an example of using POSIX source with RTEMS and not
 *  introducing RTEMS dependencies.
 */

#include <stdio.h>
#include <stdlib.h>

int main(
  int argc,
  char **argv
)
{
  printf( "\n\n*** HELLO WORLD TEST ***\n" );
  printf( "Hello World\n" );
  printf( "*** END OF HELLO WORLD TEST ***\n" );

  /*
   * Can return or call exit() directly.
   */
  return 0;
}
