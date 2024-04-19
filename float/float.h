#ifndef __FLOAT_H__
#define __FLOAT_H__

#define exp_bits  11                            // # of exponent bits
#define frac_bits 52                            // # of fraction bits

#define exp_bias  1023                          // Exponent bias
#define exp_max   2047                          // Max of exponent
#define exp_mask  0x7ff0000000000000            // Exponent mask
#define frac_mask 0x000fffffffffffff            // Fraction mask

#define isNaN(x) (((x & exp_mask) == exp_mask) &&  (x & frac_mask))
#define isInf(x) (((x & exp_mask) == exp_mask) && !(x & frac_mask))

#include <cstdint>
#include <iostream>

class float64_t {
public:
    float64_t(void);                            // Class constructor
    float64_t(const double &v);                 // Class constructor
    float64_t(const float64_t &f);              // Class copy constructor
    ~float64_t(void);                           // Class destructor

    friend std::ostream& operator<<(std::ostream &os, const float64_t &f);
    float64_t& operator=(const double &v);      // Assignment = operator
    float64_t& operator=(const float64_t &f);   // Assignment = operator
    float64_t operator-(void);                  // Unary - operator
    float64_t operator+(const float64_t &f);    // Add + operator
    float64_t operator-(const float64_t &f);    // Subtract - operator

private:
    float64_t(const uint64_t d);                // Class constructor
    uint64_t data;                              // 64-bit data
};

#endif

