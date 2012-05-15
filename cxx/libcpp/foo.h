//
//

#ifndef _foo_h
#define _foo_h

#include <iostream>

class foo {
public	:
  int i ;

  foo() : i(0) {} ;
  foo( const int i0 ) : i(i0) {};

  friend std::ostream& operator << (std::ostream&,const foo&);
  friend std::ostream& operator << (std::ostream&,const foo*);
};

#endif
