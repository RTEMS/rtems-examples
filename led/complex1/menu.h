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

#ifndef __menu_h
#define __menu_h

#ifdef __cplusplus
extern "C" {
#endif


typedef void (*test_menu_routine_t)( void );   /* Test routine */

  typedef struct {                             /* entry for a test */
  const char             *name;
  test_menu_routine_t     routine;
} Menu_entry;

  typedef struct {                             /* A menu  */
  const char   *header;
  unsigned int  number_of_entries;
  Menu_entry   *entry;
} Test_menu;

  /*
   * returns an int between min and max for value described by str.
   */

int Test_menu_Get_int_bounded(
  const char        *str,
  int                min,
  int                max
);

float Test_menu_Get_float_bounded(
  const char        *str,
  float              min,
  float              max
);

int Test_menu_Get_int(
  char              *str
);

/*
 * Returns a selection from the selection list.
 */

int Test_menu_Get_selection(
  const char        *title,
  int                select_count,
  char              *selection[]
);

/*
 * Returns a user entered string.  This is a static local variable.
 */

char *Test_menu_Get_string(
   const char        *str
);

  /*
   * Displays and processes the menu passed in.
   */

void Test_menu_run(
  Test_menu     *menu
);


#ifdef __cplusplus
}
#endif

#endif
