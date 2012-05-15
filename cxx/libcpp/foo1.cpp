#include "foo.h"

std::ostream& operator << ( std::ostream & strm, const foo *f )
{
  strm << f->i;
  return strm;
}
