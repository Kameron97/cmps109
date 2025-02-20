// $Id: ubigint.cpp,v 1.4 2017-07-09 17:41:05-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <cmath>
#include <climits>
using namespace std;

#include "ubigint.h"
#include "debug.h"


//Constructors =====

ubigint::ubigint (unsigned long that) {
   //checks to see if the number will fit in a long
   while (that > 0) {
      ubig_value.push_back(that % 10);
      //takes remainder of numbers leftover
      that = that/10;
   }
}

ubigint::ubigint (const string& that) {
   for (char digit : that) {
      ubig_value.push_back(digit - '0');
   }

   reverse(ubig_value.begin(), ubig_value.end());
}

//operator+
//got help from TA on this function
ubigint ubigint::operator+ (const ubigint& that) const {
   ubigint sum;
   unsigned int i = 0;
   int carry = 0;
   unsigned int vector = (ubig_value.size() < that.ubig_value.size()
                         ? ubig_value.size() : that.ubig_value.size());
   int digitSum = 0;

   //checks to see if an operand is 0
   if (ubig_value.size() == 0) {
      while (i < that.ubig_value.size()) {
         //if an operand is 0, then push back the stack
         sum.ubig_value.push_back(that.ubig_value.at(i));
         i++;
      }

      return sum;
   }
   //same function as above except for the 2nd operand.
   else if (that.ubig_value.size() == 0) {
      while ( i < ubig_value.size()) {
         sum.ubig_value.push_back(ubig_value.at(i));
         i++;

      }
      return sum;
   }

   //checksf  to see if the current
   // position of the digit is in bounds of vector
   while (i < vector or carry > 0) {
      if (i < ubig_value.size()){
         digitSum = digitSum + ubig_value.at(i);
      }
      if (i < that.ubig_value.size()){
         digitSum = digitSum + that.ubig_value.at(i);
      }
      digitSum = digitSum + carry;
      carry = 0;

      //checks to see how many big digistum is.
      //if greater than 9, adds +1 to carry.
      if (digitSum > 9) {
         carry = 1;
         //see what number gets left over
         digitSum = digitSum % 10;
      }

      sum.ubig_value.push_back(digitSum);
      digitSum = 0;
      i++;
   }


   //checks to see if input is larger than another
   if (ubig_value.size() > that.ubig_value.size()) {
      while ( i < ubig_value.size()) {
         //if an operand IS larger than another, 
         //pushes back the stack so they match
         sum.ubig_value.push_back(ubig_value.at(i));
         i++;
      }
   }
   else {
      while (i < that.ubig_value.size()) {
         //same logic as above.
         sum.ubig_value.push_back(that.ubig_value.at(i));
         i++;
      }
   }

   while (sum.ubig_value.size() > 0 and sum.ubig_value.back() == 0){
      sum.ubig_value.pop_back();
   }
   return sum;
}


// operator-
ubigint ubigint::operator- (const ubigint& that) const {
   if (*this < that) {
      throw domain_error ("ubigint::operator-(a<b)");
   }


   ubigint difference;
   auto this_beg = ubig_value.cbegin();
   auto this_end = ubig_value.cend();

   auto that_beg = that.ubig_value.cbegin();
   auto that_end = that.ubig_value.cend();

   int digitDiff = 0;
   int carry = 0;     //carry numerator


   while (that_beg != that_end){
        //if these 2 parts are not at the same location, then
      if (*this_beg < *that_beg ||
            (*this_beg == *that_beg && carry != 0)){
                    //must carry over function to subtract this number
         digitDiff = *this_beg + 10 - *that_beg - carry;
         carry = 1;
      }
      else {
         digitDiff = *this_beg - *that_beg - carry;
         carry = 0;
      }
      difference.ubig_value.push_back(digitDiff);
      that_beg++;
      this_beg++;

   }
    //while not at the same position
   while( this_beg != this_end){
        //if this_beg is not at front of vector
      if (*this_beg == 0 && carry != 0){
         digitDiff = *this_beg + 10 - carry;
         carry = 1;
      }
      else {
         digitDiff = *this_beg - carry;
         carry = 0;
      }
      difference.ubig_value.push_back(digitDiff);
      this_beg++;

   }
    //reset
   while (difference.ubig_value.size() > 0 
         and difference.ubig_value.back() == 0) {
      difference.ubig_value.pop_back();
   }

   return difference;

}
// operator*
ubigint ubigint::operator* (const ubigint& that) const {
   ubigint product;
   int carry = 0;
   int digit = 0;

   product.ubig_value.assign (ubig_value.size() +
         that.ubig_value.size(), 0);
            //2 loops. one goes through that function and follows
            //similar logic by hand mulitplication.
            //where position i multiplies all numbers in 2nd number
   for (size_t i = 0; i < ubig_value.size(); i++){
      carry = 0;
      for (size_t j = 0; j < that.ubig_value.size(); j++){
         digit = product.ubig_value[i+j] +
            (ubig_value[i] * that.ubig_value[j]) + carry;
         product.ubig_value[i+j] = digit%10;
         carry = digit/10;
         product.ubig_value[i + that.ubig_value.size()] = carry;
      }
   }


   while (product.ubig_value.size() > 0
          and product.ubig_value.back() == 0) {
      product.ubig_value.pop_back();
   }
   return product;
}

void ubigint::multiply_by_2(){
    (*this) = (*this) + (*this);
}

//follows eqyptian alogirthm
void ubigint::divide_by_2() {
   int remainder = 0;
   for (int i = ubig_value.size()-1; i >= 0; i--) {
      int digitQuotient = 0;
      if (remainder > 0) {
         digitQuotient += remainder;
         remainder = 0;
      }

      digitQuotient += ubig_value.at(i) / 2;

      if (ubig_value.at(i) % 2) {
         remainder = 5;
      }

      ubig_value.at(i) = digitQuotient;
   }

   while (ubig_value.size() > 0 and ubig_value.back() == 0){
     ubig_value.pop_back();
}
}



//given to us by start code.
//modified slightly
ubigint::quo_rem ubigint::udivide (const ubigint& that) const {
   ubigint zero {"0"};
   if (that == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 {"1"};
   ubigint quotient {"0"};
   ubigint divisor = that;
   ubigint remainder = *this;
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (power_of_2 > zero) {
      if (divisor <= remainder) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {quotient, remainder};
}

//operater/
ubigint ubigint::operator/ (const ubigint& that) const {
   return (udivide (that)).first;
}

//operato%
ubigint ubigint::operator% (const ubigint& that) const {
   return (udivide (that)).second;
}

//operater==
bool ubigint::operator== (const ubigint& that) const {
    //checks to see if same size,
    //if yes then
   if (ubig_value.size() == that.ubig_value.size()) {
       //loops through vector
      for (unsigned int i = 0; i < ubig_value.size(); i++) {
          //compares all results in vector to eachother.
         if (ubig_value.at(i) != that.ubig_value.at(i)) {
            return false;
         }
      }
      return true;
  }
  else {
      return false;
  }
}

bool ubigint::operator< (const ubigint& that) const {
   int first = ubig_value.size();
   int second = that.ubig_value.size();

    //check to see if 2nd has more numbers than 1st,
    //if its its true
   if (first < second){
      return true;
   }

    //if both are the same number of ints then
   else if (first == second){
      first--;
        //goes to first vector spot
      while (first > -1){
            //checks to see if highest order of int is bigger than 1sts
         if (ubig_value[first] < that.ubig_value[first]){
            return true;
         }
         else if (ubig_value[first] == that.ubig_value[first]){
            first--;
         }
         else
            break;
      }
   }
   return false;
}
//operator<<
ostream& operator<< (ostream& out, const ubigint& that) {
   int outLength = 70;
    int i = that.ubig_value.size()-1;
   if (that.ubig_value.size() > 0) {
      int charLength = 0;
      while (  i >= 0) {
         if (charLength%(outLength-1) == 0 
              and charLength != 0) out << "\\" << endl;
         out << static_cast<char>(that.ubig_value.at(i) + '0');
         charLength++;
            i--;
      }
   }
    else {
      out << '0';
   }

   return out;
}
