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
        //set the bit right before the frac bit to one before shifting it to represent the shifting of the entire significand and not just the fractional portion
        y_frac |= (uint64_t) 1 << 52; 
        y_frac >>= (x_exp - y_exp); 
        r_exp = x_exp; 
    }
    else if (x_exp < y_exp){
        x_frac |= (uint64_t) 1 << 52; 
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

    //mask to check if addition of two numbers results in overflow or underflow
    uint64_t overflowMask = 1; 
    overflowMask = overflowMask << 52;  
    uint64_t underflowMask = 1; 
    underflowMask = underflowMask << 51; 

    //add the significand of the two numbers
    r_frac = x_frac + y_frac; 

    //check if noramlization is required for overflow by checking if the 53rd bit is 1. If it is one, normalize
    //to nomralize, shift the fractional component and increment the exponent
    if ( (r_frac & overflowMask) != 0 ){
        r_frac = r_frac >> 1; 
        r_exp++; 
    }
    else if ( (r_frac & underflowMask) != 0){

    }

    // Put the calculated sign, exponent, and fraction into r.data.
    uint64_t binaryReturn = 0;
    binaryReturn |= r_sign << (exp_bits + frac_bits); //code here for clarity. the sign of two positive addition will always be 0 anyway 
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

