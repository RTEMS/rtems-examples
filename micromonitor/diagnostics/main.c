/* 
 * This file is a simple example of a "diagnostic" program that
 * could be run before a real application.
 *
 * main():
 * The main() function demonstrates a simple way to communicate the diagnostics
 * result back to the MicroMonitor script.
 */

#include "monlib.h"

int getakey(void)
{
  while(!mon_gotachar());
  return mon_getchar();
}

int
main(int argc,char *argv[])
{
  char *result = (char *)0;

  mon_printf(
    "\n\nDiagnotics Program\n"
    "Should the diagnostics return p (pass), u (unknown), f (fail)? >"
  );

  /*
   *  Run the diagnostics.  OK in this case, we are just asking the
   *  user to decide if they passed or failed.
   */
  while (1) {
     int c = getakey();
     mon_printf( "%c\n", c);
     if ( c == 'p' ) {
       result = "PASS";
       break;
     }
     if ( c == 'f' ) {
       result = "FAIL";
       break;
     }
     if ( c == 'u' ) {
       result = "UNKNOWN";
       break;
     }
     mon_printf( "Unknown response %c -- try again\n", c );
  }

  /*
   * We broke out of the loop so must have a result.
   */
  mon_setenv( "DIAGNOSTICS_RESULT", result );

  mon_printf(
    "Set DIAGNOSTICS_RESULT to %s\n"
    "At the MicroMonitor prompt: echo $DIAGNOSTICS_RESULT\n",
    mon_getenv("DIAGNOSTICS_RESULT")
  );
  
  return 0;
}
