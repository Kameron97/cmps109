// $Id: ubigint.h,v 1.1 2017-07-09 17:17:53-07 - - $

#ifndef __UBIGINT_H__
#define __UBIGINT_H__

#include <exception>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;

#include "debug.h"
#include "relops.h"

class ubigint {
   friend ostream& operator<< (ostream&, const ubigint&);
   private:
       using quo_rem = pair<ubigint, ubigint>;
      using unumber = unsigned long;
      unumber uvalue {};
      using udigit_t = unsigned char;
using ubigvalue_t = vector<udigit_t>;
ubigvalue_t ubig_value;
quo_rem udivide (const ubigint&) const;
     void multiply_by_2();
     void divide_by_2();
   public:


      ubigint() = default; // Need default ctor as well.
      ubigint (unsigned long);
      ubigint (const string&);

      ubigint operator+ (const ubigint&) const;
      ubigint operator- (const ubigint&) const;
      ubigint operator* (const ubigint&) const;
      ubigint operator/ (const ubigint&) const;
      ubigint operator% (const ubigint&) const;

      bool operator== (const ubigint&) const;
      bool operator<  (const ubigint&) const;
};

#endif
