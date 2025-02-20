// $Id: hello.h,v 1.1 2017-06-14 16:42:35-07 - - $

#ifndef __HELLO_H__
#define __HELLO_H__

#include <iostream>
#include <string>

class hello {
	private:
		std::string message {"Hello, World!"};
	public:
		hello();                         // default constructor
		hello (const hello&);            // copy constructor
		hello& operator= (const hello&); // copy operator=
		hello (hello&&);                 // move constructor
		hello& operator= (hello&&);      // move operator=
		~hello();                        // destructor
		hello (const string&);           // alternate constructor
		void say (ostream&);             // member function
};

#endif

