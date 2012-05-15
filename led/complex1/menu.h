/*
 *  This is a general set of routines to provide a user interface
 *  for testing.
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
