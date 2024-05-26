#include "float.h"

using namespace std;

int main(void) {
    /******************************************
     * DO NOT MODIFY THE CODE BELOW THIS LINE *
     ******************************************/
    cout << "======== [Tests for grading] ========" << endl;

    //double x, y; 
    float64_t x, y;
    uint64_t t = 0, u = 0;
    
    // Add two positive numbers.
    x = 3.875;
    y = 2.625;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two numbers of different signs.
    x = -10.1;
    y = 1.32;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two negative numbers.
    x = -1.21e-1;
    y = -4.252e1;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two large numbers.
    x = 1.39e+52;
    y = 8.24e+49;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two values of opposite signs.
    x = 2.024e24;
    y = -x;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two denormalized numbers.
    u = 1;                                          // Set the LSB.
    x = *((double*)&u);
    u <<= 2;                                        // Set the third LSB
    y = *((double*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add max and min.
    u = (uint64_t(0x7fe) << frac_bits) + frac_mask; // Max (exp = 2046, all frac bits set)
    x = *((double*)&u);
    u = uint64_t(1) << frac_bits;                   // Min (exp = 1, frac = 0)
    y = *((double*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add infinity.
    u = uint64_t(0x7ff) << frac_bits;               // Infinity (exp = 2047, frac = 0)
    x = *((double*)&u);
    y = 3.14159;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add a denormalized number and a normalized number.
    u = uint64_t(1) << (frac_bits - 1);             // Denormalized (exp = 0, MSB of frac set)
    x = *((double*)&u);
    u = uint64_t(1) << frac_bits;                   // Normalized   (exp = 1, frac = 0)
    y = *((double*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add two max numbers.
    u = (uint64_t(0x7fe) << frac_bits) + frac_mask; // Max (exp = 2046, all frac bits set)
    x = *((double*)&u);
    y = x;
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Add infinity and NaN.
    u = uint64_t(0x7ff) << frac_bits;               // Infinity (exp = 2047, frac = 0)
    x = *((double*)&u);
    u = uint64_t(0xfff) << (frac_bits - 1);         // NaN (exp = 2047, MSB of frac set)
    y = *((double*)&u);
    cout << "Test #" << ++t << ": " << x << " + " << y << " = " << x + y << endl;

    // Subtract two positive numbers.
    x = 1.344;
    y = 9.227;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract the same number.
    x = -8.242;
    y = x;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract a number from zero.
    x = 0;
    y = 3.582;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;
   
    // Subtract two small numbers.
    x = -8.85e-39;
    y = -1.85e-40;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract -infinity.
    x = -1.23;
    u = uint64_t(0xfff) << frac_bits;               // -Infinity (sign = 1, exp = 2047, frac = 0)
    y = *((double*)&u);
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract NaN;
    x = 4.641;
    u = uint64_t(0xfff) << (frac_bits - 1);         // NaN (exp = 2047, MSB of frac set)
    y = *((double*)&u);
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract a normalized number from a denormalized number.
    u = uint64_t(1) << (frac_bits - 1);             // Denormalized (exp = 0, MSB of frac set)
    x = *((double*)&u);
    u = uint64_t(1) << frac_bits;                   // Normalized   (exp = 1, frac = 0)
    y = *((double*)&u);
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract the denormalized numbers of opposite signs.
    u = uint64_t(1) << (frac_bits - 1);             // Denormalized (exp = 0, MSB of frac set)
    x = *((double*)&u);
    y = -x;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    // Subtract infinity from -infinity.
    u = uint64_t(0x7ff) << frac_bits;               // Infinity (exp = 2047, frac = 0)
    x = *((double*)&u);
    y = -x;
    cout << "Test #" << ++t << ": " << x << " - " << y << " = " << x - y << endl;

    cout << "======== [End of tests] ========" << endl;
    /******************************************
     * DO NOT MODIFY THE CODE ABOVE THIS LINE *
     ******************************************/
    return 0;
}

