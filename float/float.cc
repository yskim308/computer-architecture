#include "float.h"

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
    // bool x_sign    = data >> (exp_bits + frac_bits);
    // int64_t x_exp  = (data & exp_mask) >> frac_bits;
    // int64_t x_frac = data & frac_mask;
    
    // An example to extract the sign, exponent, and fraction of y (arg).
    // bool y_sign    = y.data >> (exp_bits + frac_bits);
    // int64_t y_exp  = (y.data & exp_mask) >> frac_bits;
    // int64_t y_frac = y.data & frac_mask;

    // Put the calculated sign, exponent, and fraction into r.data.
    float64_t r;
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

