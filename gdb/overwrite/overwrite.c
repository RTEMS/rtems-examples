/*
 *  Simple test program to demonstrate memory overwrite.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  uint32_t    TooShortArray[8];
  uint32_t    RandomVariable;
  uint32_t    CorruptedVariable;
} Struct_t;

Struct_t S;

void writeEntry(int index, uint32_t value);

int main(
  int    argc,
  char **argv
)
{
  puts( "\n\n*** MEMORY CORRUPTION EXAMPLE ***" );

  /* initialize global variables */
  memset( &S, 0, sizeof(S) );
  S.RandomVariable = 0x1234;
  S.CorruptedVariable = 0x5678;

  printf( 
    "&TooShortArray[0] = %p\n"
    "&TooShortArray[7] = %p\n"
    "&RandomVariable = %p\n"
    "&CorruptedVariable = %p\n",
    &S.TooShortArray[0],
    &S.TooShortArray[7],
    &S.RandomVariable,
    &S.CorruptedVariable
  );
  /* write past end of array */
  writeEntry( 8, 0xdeadf00d );
  writeEntry( 9, 0x0badd00d );

  printf( 
    "RandomVariable = 0x%08x\n"
    "CorruptedVariable = 0x%08x\n",
    S.RandomVariable,
    S.CorruptedVariable
  );
  puts( "*** END OF MEMORY CORRUPTION EXAMPLE ***" );
  exit( 0 );
}

void writeEntry(int index, uint32_t value)
{
  S.TooShortArray[index] = value;
}
