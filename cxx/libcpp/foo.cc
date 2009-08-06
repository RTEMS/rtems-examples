
#include "foo.h"

ostream& operator << ( ostream & strm, const foo &f)
{
  strm << f.i ;
  return strm ;
}
