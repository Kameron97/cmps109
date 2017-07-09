// $Id: bigint.cpp,v 1.76 2016-06-14 16:34:24-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (long that): uvalue (that), is_negative (that < 0) {
	DEBUGF ('~', this << " -> " << uvalue)
}

bigint::bigint (const ubigint& uvalue, bool is_negative):
	uvalue(uvalue), is_negative(is_negative) {
	}

bigint::bigint (const string& that) {
	is_negative = that.size() > 0 and that[0] == '_';
	uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
	return *this;
}

bigint bigint::operator- () const {
	return {uvalue, not is_negative};
}
//checks to see if both values are negative
//if both negative, add together than add negative sign to it
bigint bigint::operator+ (const bigint& that) const {
	if (is_negative and that.is_negative){
		return {uvalue+that.uvalue, true};
	}
	//checks to see if one of the numbers is negative
	//if they are equal in value, return 0
	//if first>2nd, do return (1st-2nd, negative)
	//if 2nd> 1st do return (1st-2nd, positive)
	//vice versa for 2nd number
	else if (is_negative or that.is_negative){
		if (uvalue > that.uvalue){
			if (is_negative){
				return {uvalue - that.uvalue, true};
			}
			else{
				return {uvalue - that.uvalue, false};
			}
		}
		//that.uvalue >uvalue
		else if (that.uvalue > uvalue){
			if (that.is_negative){
				return {that.uvalue - uvalue, true};
			}
			else{
				return {that.uvalue - uvalue, false};
			}

		}
	else{
			return uvalue - that.uvalue;
		}
	}

	//if both positive, just add.
	ubigint result = uvalue + that.uvalue;
	return result;
}

bigint bigint::operator- (const bigint& that) const {
	//checks to see if both numbers are negative
	//if 1st>2nd, then it will be negative
	if (is_negative and that.is_negative){
		if (uvalue > that.uvalue ){
			return {uvalue - that.uvalue, true};
		}
		//if 2nd>1st, then must be positive
		else if (that.uvalue > uvalue ){
			return {that.uvalue - uvalue, false};
		}
		else {
			return 0;
		}
	}

	//if 1st or 2nd are negative, then it will add them together
	else if (is_negative or that.is_negative){
		if (is_negative){
			return {uvalue + that.uvalue, true};
		}
		else{
			return {uvalue + that.uvalue, false};
		}
	}

	ubigint result = uvalue - that.uvalue;
	return result;
}

bigint bigint::operator* (const bigint& that) const {
	//checks to see if both inpputs are negative or  positive
	if( (is_negative and that.is_negative) or
			(not is_negative and not that.is_negative) ){
		bigint result = uvalue * that.uvalue;
		return result;
	}
	else{
		return {uvalue * that.uvalue, true};
	}
}


//same logic as operator*
bigint bigint::operator/ (const bigint& that) const {
	//checks to see if both inpputs are negative or  positive
	if( (is_negative and that.is_negative) or
			(not is_negative and not that.is_negative) ){
		bigint result = uvalue / that.uvalue;
		return result;
	}
	else{
		return {uvalue / that.uvalue, true};
	}
}

//same logic as operator*
bigint bigint::operator% (const bigint& that) const {
	//checks to see if both inpputs are negative or  positive
	if( (is_negative and that.is_negative) or
			(not is_negative and not that.is_negative) ){
		bigint result = uvalue % that.uvalue;
		return result;
	}
	else{
		return {uvalue  % that.uvalue, true};
	}
}

bool bigint::operator== (const bigint& that) const {
	return is_negative == that.is_negative and uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
	if (is_negative != that.is_negative) return is_negative;
	return is_negative ? uvalue > that.uvalue
		: uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << (that.is_negative ? "-" : "")
              << that.uvalue;
}
