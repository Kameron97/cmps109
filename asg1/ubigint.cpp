// $Id: ubigint.cpp,v 1.14 2016-06-23 17:21:26-07 - - $

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
	unsigned int vector = (ubig_value.size() < that.ubig_value.size() ? ubig_value.size() : that.ubig_value.size());
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

	//checksf  to see if the current position of the digit is in bounds of vector
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
			//if an operand IS larger than another, pushes back the stack so they match
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
	int carry = 0;


	while (that_beg != that_end){

		if (*this_beg < *that_beg ||
				(*this_beg == *that_beg && carry != 0)){
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

	while( this_beg != this_end){

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
	while (difference.ubig_value.size() > 0 and difference.ubig_value.back() == 0) { // delete the extra 0s (eg, 00043 = 43).
		difference.ubig_value.pop_back(); //pop_back() the extra 0s out of the vector
	}

	return difference;

}
// operator*
// this function multiplies two ubigint types
ubigint ubigint::operator* (const ubigint& that) const {
	ubigint product;
	int carry = 0;
	int digit = 0;

	product.ubig_value.assign (ubig_value.size() +
			that.ubig_value.size(), 0);

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


	while (product.ubig_value.size() > 0 and product.ubig_value.back() == 0) {
		product.ubig_value.pop_back();
	}
	return product;
}

void ubigint::multiply_by_2(){
    (*this) = (*this) + (*this);
}

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

	while (ubig_value.size() > 0 and ubig_value.back() == 0) ubig_value.pop_back();
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

ubigint ubigint::operator/ (const ubigint& that) const {
	return (udivide (that)).first;
}

ubigint ubigint::operator% (const ubigint& that) const {
	return (udivide (that)).second;
}

bool ubigint::operator== (const ubigint& that) const {
	size_t iter {0};
	//Check if both numbers same size
	if (ubig_value.size() == that.ubig_value.size()){
		for (int left : ubig_value){
			int right = that.ubig_value[iter];
			//If same size check each digit
			//If reach end then return true
			if (left == right){
				if (left == right and ubig_value.size() == iter + 1){
					return true;
				}
			}
			else
				break;
			++iter;
		}
	}
	return false;
}

bool ubigint::operator< (const ubigint& that) const {
	int Lsize = ubig_value.size();
	int Rsize = that.ubig_value.size();
	//If left size < right size, return true
	if (Lsize < Rsize){
		return true;
	}//Are numbers same size?
	else if (Lsize == Rsize){
		--Lsize;
		while (Lsize > -1){
			//Start comparing from high to low order digits
			if (ubig_value[Lsize] < that.ubig_value[Lsize]){
				return true;
			}//If digits same, proceed to next lower digit
			else if (ubig_value[Lsize] == that.ubig_value[Lsize]){
				--Lsize;
			}//If digits different then left must be > right
			else
				break;
		}
	}
	return false;
}

ostream& operator<< (ostream& out, const ubigint& that) {
	int outLength = 70;
	if (that.ubig_value.size() > 0) {
		int charCount = 0;
		for (int i = that.ubig_value.size()-1; i >= 0; i--) {
			if (charCount%(outLength-1) == 0 and charCount != 0) out << "\\" << endl;
			out << static_cast<char>(that.ubig_value.at(i) + '0');
			charCount++;
		}
	} else {
		out << '0';
	}

	return out;
}
