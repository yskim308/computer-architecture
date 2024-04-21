#include "float.h"
#include <cstdint>

using namespace std;

// Class constructor
float64_t::float64_t(void) : data(0) { /* Nothing to do */ }

// Class constructor
float64_t::float64_t(const double &v) : data(*((uint64_t*)&v)) { /* Nothing to do */ }

// Class constructor (private)
float64_t::float64_t(const uint64_t d) : data(d) { /* Nothing to do */ }
   
// Class copy constructor
float64_t::float64_t(const float64_t &f) : data(f.data) { /* Nothing to do */ }

// Class destructor
float64_t::~float64_t(void) { /* Nothing to do */ }

// cout << operator
ostream& operator<<(ostream &os, const float64_t &f) { os << *((double*)&f.data); return os; }

// Assignment = operator
float64_t& float64_t::operator=(const double &v) { data = *((uint64_t*)&v); return *this; }

// Assignment = operator
float64_t& float64_t::operator=(const float64_t &f) { data = f.data; return *this; }

// Unary - operator
float64_t float64_t::operator-(void) { return float64_t(data ^ (uint64_t(1) << (exp_bits + frac_bits))); }





/*******************************************************************
 * EEE3530 Assignment #3: Floating-point numbers                   *
 * Implement the double-precision floating-point add and subtract  *
 * operators below. The functions should perform bit-level         *
 * operations to produce the results.                              *
 *******************************************************************/

// Add + operator
float64_t float64_t::operator+(const float64_t &y) {
    /***************************************************************
     * EEE3530 Assignment #3                                       *
     * Implement the double-precision floating-point add function. *
     ***************************************************************/

    // An example to extract the sign, exponent, and fraction of x (*this).
    // note: changing all to 64bit integers so they can be shifted and manipulated accordingly 
    uint64_t x_sign    = data >> (exp_bits + frac_bits);
    uint64_t x_exp  = (data & exp_mask) >> frac_bits;
    uint64_t x_frac = data & frac_mask;
   
    // An example to extract the sign, exponent, and fraction of y (arg).
    uint64_t y_sign    = y.data >> (exp_bits + frac_bits);
    uint64_t y_exp  = (y.data & exp_mask) >> frac_bits;
    uint64_t y_frac = y.data & frac_mask;
   
    //declare the digits to return 
    uint64_t r_sign, r_frac, r_exp;

    //shift the smaller number's fractional component by the difference of the exponent values
    //and store the larger exponent into the return exponent value 
    if (x_exp > y_exp){
        //if the number is NOT denormalized, add implicit 1 before shifting. Otherwise just shift (normal: 1.xxx >> 0.1xxx, denormalized: 0.xxx >> 0.0xxx)
        if (y_exp !=0 ){
            y_frac |= (uint64_t) 1 << 52; 
        }
        y_frac >>= (x_exp - y_exp);
        r_exp = x_exp; 
    } //doing same for the case where x_exp is less than y_exp 
    else if (x_exp < y_exp){
        if (x_exp != 0){
            x_frac |= (uint64_t) 1 << 52;
        }
        x_frac >>=  (y_exp - x_exp); 
        r_exp = y_exp; 
    }
    else { //if both exponent equal, nothing to do 
        r_exp = x_exp; 
    } 

    //check the resulting sign: the resulting sign is the sign of the number with the larger absolute value 
    //if the two exponent values are equal, check which fractional value is larger and set the sign accordingly 
    //otherwise, check which exponent value is larger and set the sign accordingly 
    if (x_exp == y_exp){
        r_sign = x_frac > y_frac ? x_sign : y_sign; 
    }
    else {
        r_sign = x_exp > y_exp ? x_sign : y_sign; 
    }


    //switch to two's complement addition if the two signs are not equal (XOR with 111.... to invert, then add one, then throw away overflow) 
    if (x_sign == 1 && y_sign == 0){
        x_frac = ((x_frac ^ frac_mask) + 1) & frac_mask;
    }
    else if (y_sign == 1 && x_sign == 0){
        y_frac = ((y_frac ^ frac_mask) + 1) & frac_mask;
    }

    //add the significand of the two numbers
    r_frac = x_frac + y_frac; 
    //if two's complement form was used, then make sure to invert result and add one (same process as before)
    if (r_sign == 1 && x_sign != y_sign){
        r_frac = ((r_frac ^ frac_mask) + 1) & frac_mask; 
    }

    //cout << endl << "r_exp before normalizign: " << r_exp << endl;
    //if the 53rd bit is not zero, it indicates overflow, so shift right and increment exponent
    if ( (r_frac >> 52) != 0 ){
        r_frac |= (uint64_t) 1 >> 53; //make sure that we are doing arithmetic shift (data is 64 bit so right shifting from 52 would fill with 0 not 1)
        r_frac >>= 1; 
        r_exp ++; 
        //if after shifting, the exponent value exceeds the maximum, then we have went over the maximum representable values, (infinity) thus fill exp and frac accordingly
        if( r_exp >= 2047){
            r_exp = 2047; 
            r_frac = 0; 
        }
    }

    //if the fractional part is 0, then underflow has occured. Two cases;
    if (r_frac == 0 && x_sign == y_sign && r_exp != 2047) { //case one: underflow is result of same-sign addition. Then set all bits to 1, decrement exp, and invert sign 
        r_frac = frac_mask;
        r_exp--;
        r_sign = ~r_sign;
    }
    else if (r_frac == 0 && x_sign != y_sign && r_exp != 2047) { //case two: underflow is result of subtraction. Then it represents 0. 
        r_frac = 0;
        r_exp = 0;
        r_sign = ~r_sign;
    }
    
    //cout << endl << "r_sign: " << r_sign << endl << "r_exp: " << r_exp << endl << "r_frac: " << r_frac << endl;
    // Put the calculated sign, exponent, and fraction into r.data.
    uint64_t binaryReturn = 0; //empty return 64bit data
    binaryReturn |= r_sign << (exp_bits + frac_bits);  
    binaryReturn |= r_exp << (frac_bits);
    binaryReturn |= r_frac; 

    float64_t r = *(float64_t*)&binaryReturn; 
    return r;
    
}

// Subtract - operator
float64_t float64_t::operator-(const float64_t &y) {
    /***************************************************************
     * EEE3530 Assignment #3                                       *
     * Implement the double-precision floating-point sub function. *
     ***************************************************************/

    // Put the calculated sign, exponent, and fraction into r.data.
    float64_t r;
    return r;
}

