/*
 *  Start Test Example
 */

#if defined(USE_START_TEST)

#define __need_getopt_newlib
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtems/shell.h>

#define COMMAND_LINE_ARGUMENTS_FILE "arguments"

#define ARGV_LENGTH 80
#define ARGV_LIMIT  32

int     Argc;
char   *Argv[ARGV_LIMIT];
char    Argv_String[ ARGV_LENGTH ];

void parse_arguments(void)
{
  FILE   *in;
  char   *cStatus;
  size_t  length;
  int     sc;

  in = fopen( COMMAND_LINE_ARGUMENTS_FILE, "r" );
  if ( !in ) {
    fprintf( stderr, "Unable to open %s\n", COMMAND_LINE_ARGUMENTS_FILE );
    exit(1);
  }

  cStatus = fgets( Argv_String, sizeof(Argv_String), in );
  if ( cStatus == NULL ) {
    fprintf( stderr, "Unable to read %s\n", COMMAND_LINE_ARGUMENTS_FILE );
    exit(1);
  }

  // If the last line does not have a CR, then we don't want to
  // arbitrarily clobber an = instead of a \n.
  length = strlen(Argv_String);
  if ( Argv_String[ length - 1] != '\n' ) {
    fprintf(
      stderr,
      "Line appears to be too long in %s\n",
      COMMAND_LINE_ARGUMENTS_FILE
    );
    exit(1);
  }

  Argv_String[ length - 1] = '\0';

  sc = rtems_shell_make_args(
    &Argv_String[0],
    &Argc,
    &Argv[1],
    ARGV_LIMIT - 1
  );
  if ( sc ) {
    fprintf( stderr, "Error parsing arguments\n" );
    exit(1);
  }

  /* Since we added the program name ourselves and started at 0 */
  Argc++;
  fclose(in);
}

void print_arguments(void)
{
  int i;

  for ( i=0 ; i<Argc ; i++ ) {
    fprintf( stderr, "argc[%d] = %s\n", i, Argv[i] );
  }

}

void start_test(void)
{
  Argv[0] = "my_program";
  parse_arguments();
  print_arguments();
  /* If this were a real user test, we would invoke the main here */
}
#endif
