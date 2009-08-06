/* 
 * This file is a simple example of an application that could be run
 * on top of the monitor.
 *
 * Cstart():
 * The Cstart() function depends on the setting of MONCOMPTR in config.h.
 * It demonstrates the use of monConnect and the first mon_XXX function
 * typically called by an application, mon_getargv().
 */

#include "monlib.h"
#include "tfs.h"
#include "cfg.h"

extern int main(int, char**);

int Cstart(void)
{
  char  **argv;
  int    argc;

  /* Connect the application to the monitor.  This must be done
   * prior to the application making any other attempts to use the
   * "mon_" functions provided by the monitor.
   */
  monConnect((int(*)())(*(unsigned long *)MONCOMPTR),(void *)0,(void *)0);

  /* When the monitor starts up an application, it stores the argument
   * list internally.  The call to mon_getargv() retrieves the arg list
   * for use by this application...
   */
  mon_getargv(&argc,&argv);

  /* Call main, then exit to monitor.
   */
  main(argc,argv);

  /* mon_appexit(0); */

  /* Won't get here. */
  return(0);
}

#if 0
/* CstartAlt():
 * Demonstrates the use of the "call -a" command in uMon. 
 * For example, if for some reason you wanted to do this...
 * Load the application then load the symtbl file using
 * "make TARGET_IP=1.2.3.4 sym", then issue these commands:
 *
 *  tfs -v ld app
 *  call -a %CstartAlt one two three
 *
 * The "call -a" command in uMon correctly sets up the function
 * call parameters so that the following function would see 4
 * arguments (including arg0), with argv[1] thru argv[3] being
 * pointers to each of the number strings (i.e. "one", "two", "three")
 * and argv[0] being the ascii-coded-hex address of the function
 * CstartAlt.
 */
int
CstartAlt(int argc, char *argv[])
{
  monConnect((int(*)())(*(unsigned long *)MONCOMPTR),(void *)0,(void *)0);
  main(argc,argv);
  mon_appexit(0);
  return(0);
}
#endif
