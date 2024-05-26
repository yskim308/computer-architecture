#include "float.h"
#include <cstdint>
#include <sys/types.h>

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

    //bool flags to check if either x or y is denoramlized.  
    bool x_is_denorm, y_is_denorm; 
    x_is_denorm = (x_exp == 0 && x_frac !=0) ? true: false;
    y_is_denorm = (y_exp == 0 && y_frac !=0) ? true: false;

    //if either number is nan, immediately return nan as a result (no need to do any calculations)
    if ((x_exp == 2047 && x_frac !=0) || (y_exp == 2047 && y_frac!=0)){
        r_sign = 0; 
        r_frac = 1; 
        r_exp = 2047; 
        uint64_t binaryReturn = 0; //empty return 64bit data
        binaryReturn |= r_sign << (exp_bits + frac_bits);  
        binaryReturn |= r_exp << (frac_bits);
        binaryReturn |= r_frac; 

        float64_t r = *(float64_t*)&binaryReturn; 
        return r;
    }

    //shift the smaller number's fractional component by the difference of the exponent values
    //and store the larger exponent into the return exponent value 
    if (x_exp > y_exp){
        //if the number is NOT denormalized, add implicit 1 before shifting by the difference in exponent.  
        if (y_exp != 0){
            y_frac |= (uint64_t) 1 << 52; 
        }

        if (y_is_denorm){ //if the number is denormalized, shift by -1 to ensure proper alignment of bits
            y_frac >>= (x_exp - y_exp - 1);
        }
        else{ //if number is normal, shift by the different of exponents
            y_frac >>= (x_exp - y_exp);
        }
        r_exp = x_exp; //set result exponent as exponent of larger exponent 
    } //doing same for the case where x_exp is less than y_exp 
    else if (x_exp < y_exp){
        if (x_exp != 0){
            x_frac |= (uint64_t) 1 << 52;
        }

        if (x_is_denorm){
            x_frac >>= (y_exp - x_exp - 1);
        }
        else{
            x_frac >>= (y_exp - x_exp);
        }
        r_exp = y_exp; 
    }
    else { //if both exponent equal, nothing to do (no shifting to needs to be completed)
        r_exp = x_exp; 
    } 

    //check the absolute value of each of the numbers and assign r_sign as the sign of the larger absolute value number
    int checkx, checky; //checkx and checky are for setting both values to positive 
    checkx = x_sign == 1 ? -1: 1; 
    checky = y_sign == 1 ? -1: 1; 
    r_sign = (checkx)* *(double*)&data > (checky)* *(double*)&y.data ? x_sign : y_sign; 

    //complete the correct arithmetic operation depending on the sign of x and y 
    if (x_sign == 1 && y_sign == 0){
        r_frac = y_frac - x_frac;
    }
    else if (y_sign == 1 && x_sign == 0){
        r_frac = x_frac - y_frac;
    }
    else { //if addition, order of operations does not matter
        r_frac = x_frac + y_frac;
    }

    //if there is subtraction of numbers of different sign, ensure two's complement conversion is completed
    if (r_sign == 1 && x_sign != y_sign){
        r_frac = ((r_frac ^ frac_mask) + 1) & frac_mask; 
    }

    if (x_is_denorm && y_is_denorm) { //if both numbers denormalized, overflow means a normalized number. Thus shift by one and increment exponent. 
        if ((r_frac >> 52) == 1){
            r_exp++;
        }
    } 
    else if ((x_is_denorm != y_is_denorm) && r_sign == 1){ //if both numbers are different (noramlized and denormalized) set exponent to zero if no overflow
        if ((r_frac >>52) == 0){ //this essentially sets the resultant number as another denormalized number 
            r_exp = 0;
        }
    }
    else if ( (r_frac >> 52) != 0 ){ //otherwise, if both are normal, do as follows
        r_frac |= (uint64_t) 1 >> 53; //make sure that we are doing arithmetic shift (data is 64 bit so right shifting from 52 would fill with 0 not 1)
        r_frac >>= 1; //shift by one and increment exp
        r_exp ++; 
        //if after shifting, the exponent value exceeds the maximum, then we have went over the maximum representable values, (infinity) thus fill exp and frac accordingly
        if( r_exp >= 2047){
            r_exp = 2047; 
            r_frac = 0; 
        }
    } //lastly check for underflow for normal numbers. If the 52nd bith is 1 but the 53rd bit is 0, then shift left and decrement exponent
    else if ( (r_frac >> 51) == 1 && !x_is_denorm && !y_is_denorm && x_frac !=0 && y_frac !=0){
        r_frac <<= 1;
        r_exp--;
    }

   
    //if the fractional compoenent is zero and it is the result of subtraction, then it must represent zero.
    if (r_frac == 0 && x_sign != y_sign && r_exp != 2047) { // for zero, all bits must be 0 
        r_frac = 0;
        r_exp = 0;
        r_sign = 0;
    }
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
     * Implement the double-precision floating-point sub function. 
     ***************************************************************/
    //make a copy of the y data, set the invert the MSB (sign bit) and then call the + operator overload
    float64_t y_copy = y;
    y_copy.data ^= (uint64_t) 1 << (exp_bits + frac_bits) ;
    float64_t r; 
    r = float64_t::operator+(y_copy); 
    return r; 
}

