/* SPDX-License-Identifier: BSD-2-Clause */

/*
 *  This is a general set of routines to provide a user interface
 *  for testing.
 */

/*
 * COPYRIGHT (c) 2026.
 * On-Line Applications Research Corporation (OAR).
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


#include <bsp.h>
#if !defined(BSP_SMALL_MEMORY)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "menu.h"

int Test_menu_Get_int_bounded(
  const char        *str,
  int                min,
  int                max
)
{
  int                          selection = 0x80000000;
  char                         Message[ 80 ];
  int                          i=0;
  bool                         done = false;
  char                        *temp;

  assert( min <= max );
  do {
    if (i != 0)
      printf("  Invalid Entry\n");

    printf("\n %s (%d:%d) > ", str, min, max );
    fflush( stdout );
    fflush( stdin );
    fgets( Message, 80, stdin );
    if ((Message[0] != '\0') && (Message[0] != '\n')) {
      selection = strtol( Message, &temp, 10 );
      if ((*temp == '\0') || (*temp == '\n') || (*temp == '\r') )
        if (!( selection <  min  || selection > max ))
          done = true;
    }
    i++;
  }
  while  (!done );

  return selection;
}
int Test_menu_Get_int(
  const char        *str
)
{
  int                          selection = 0x80000000;
  char                         Message[ 80 ];
  int                          i=0;
  bool                         done = false;
  char                        *temp;

  do {
    if (i != 0)
      printf("  Invalid Entry\n");

    printf("\n %s > ", str );
    fflush( stdout );
    fflush( stdin );
    fgets( Message, 80, stdin );
    selection = strtol( Message, &temp, 10 );
    if ( *temp == '\0' )
      done = true;
    i++;
  }
  while  (!done );

  return selection;
}

float Test_menu_Get_float_bounded(
  const char        *str,
  float              min,
  float              max
)
{
  double                       selection = -99999999.0;
  char                         Message[ 80 ];
  int                          i=0;
  bool                         done = false;
  char                        *temp;

  assert( min <= max );

  do {
    if (i != 0)
      printf("  Invalid Entry\n");

    printf("\n %s (%f:%f) > ", str, min, max );
    fflush( stdout );
    fflush( stdin );
    fgets( Message, 80, stdin );
    selection = strtod( Message, &temp );
    if (( *temp == '\0' ) || ( *temp == '\n' ))
      if (!( selection <  min  || selection > max ))
        done = true;
    i++;
  }
  while  (!done );

  return (float)selection;
}

int Test_menu_Get_selection(
  const char        *title,
  int                count,
  char              *selection[]
)
{
  int      i;
  int      value;

  printf("\n     %s     \n     ", title);
  for(i=0; i<(int)strlen(title);i++)
    printf("=");
  printf("\n");

  if ( count<21 ) {
    for (i=0; i<count; i++)
      printf("\n(%d) %s", i, selection[i] );
  } else {
    for (i=0; i<count; ) {
      printf("\n(%2d) %-30s", i, selection[i] );
      if ( ++i >= count ) break;
      printf("    (%2d) %-30s", i, selection[i] );
      i++;
    }
  }

  value = Test_menu_Get_int_bounded(
    "    Enter your selection> ",
    0,
    count-1
  );

  return value;
}


char *Test_menu_Get_string(
   const char        *str
)
{
  static char               Message[ 500 ];
  int    last;

  printf("\n Enter %s : ", str);
  fgets( Message, 500, stdin );
  last = strlen( Message );

  if (Message[last-1] == '\n')
    Message[last-1] = '\0';

  return Message;
}

void Test_menu_run(
  Test_menu     *menu
)
{
  int                          selection;
  bool                         done = false;
  int                          i;

  while ( !done ) {

    printf("\n     %s     \n     ", menu->header);
    for(i=0; i<(int)strlen(menu->header);i++)
      printf("=");
     printf("\n");

    for (i=0; i<(int)menu->number_of_entries; i++)
      printf("\n     (%d) %s", i+1, menu->entry[i].name );

    printf("\n     (0) Exit\n");

    selection = Test_menu_Get_int_bounded(
      "    Enter your selection> ",
      0,
      menu->number_of_entries
    );

    if ( selection == 0 ) {
       // printf( "Exiting %s\n", menu->header );
       done = true;
    } else {
      (*menu->entry[ selection - 1 ].routine)();
    }
  }
}

#endif
