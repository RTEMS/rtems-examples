
#include "foo.h"

ostream& operator << ( ostream & strm, const foo &f, const foo &f1 )
{
  strm << f.i ;
  return strm ;
}
