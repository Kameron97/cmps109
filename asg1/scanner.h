// $Id: scanner.h,v 1.1 2017-07-09 17:17:53-07 - - $

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <iostream>
#include <utility>
using namespace std;

#include "debug.h"

enum class tsymbol {SCANEOF, NUMBER, OPERATOR};

struct token {
   tsymbol symbol;
   string lexinfo;
   token (tsymbol sym, const string& lex = string()):
          symbol(sym), lexinfo(lex){
   }
};

class scanner {
   private:
      istream& instream;
      int nextchar {instream.get()};
      bool good() { return nextchar != EOF; }
      char get();
   public:
      scanner (istream& instream = cin): instream(instream) {}
      token scan();
};

ostream& operator<< (ostream&, tsymbol);
ostream& operator<< (ostream&, const token&);

#endif

