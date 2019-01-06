#include <iostream>
#include <iomanip>
#include <math.h>
#include <limits>

#include "conversion.h"

void printDoubleComponents(double f);
void testltoa(long l);
void testdtoa();

void testMagnitudes();
void testLog2ToLog10Approximation();
void testMangitudeApproximationForDoubles();
void testLengthOfLong();

int main() {
    //testltoa(123456789123456789);
    //testltoa(0x8000000000000000);

    // testdtoa();
    testMagnitudes();

    // testLog2ToLog10Approximation();
    // testLengthOfLong();
    // testMangitudeApproximationForDoubles();
}


void testltoa(long l) {
    char* out = new char[256];
    
    int length = ltoa(out, l);
    std::cout << "length: " << length << std::endl;
    out[length] = 0;

    std::cout << "long" << std::endl;
    std::cout << "std: " <<  l << std::endl;
    std::cout << "me:  " << out << std::endl;

    delete[] out;
}


void testdtoa() {
    char* out = new char[256];
    double tests[] = {
        0.0, -0.0, INFINITY, -INFINITY, NAN, -NAN,
        123.456,
        100.005,
        1e10,
        1e-308,
        9.999,
        0.123456789,
        -987.654e10,
        123.321e-10,
    };

    std::cout << std::left << std::setw(20) << "C++ built-in";
    std::cout << std::left << std::setw(20) << "default";
    std::cout << std::left << std::setw(20) << "fixed-point";
    std::cout << std::left << std::setw(20) << "2 decimal digits";
    std::cout << std::endl;

    std::cout << std::setfill('_') << std::setw(80) << "" << std::endl << std::setfill(' ');

    for (double d : tests) {
        std::cout << std::left << std::setw(20) << d;

        int length;
        
        length = dtoa(out, d);
        out[length] = 0;
        std::cout << std::left << std::setw(20) << out;

        length = dtoa(out, d, 0);
        out[length] = 0;
        std::cout << std::left << std::setw(20) << out;

        length = dtoa(out, d, 2);
        out[length] = 0;
        std::cout << std::left << std::setw(20) << out;

        std::cout << std::endl;
    }

    delete[] out;
}


void testMagnitudes() {
    char* out = new char[256];
    std::fill(out, out + 256, 'x');

    std::cout << std::left << std::setw(20) << "C++ built-in";
    std::cout << std::left << std::setw(20) << "custom";
    std::cout << std::endl;

    std::cout << std::setfill('_') << std::setw(80) << "" << std::endl << std::setfill(' ');

    for (double d = 1e-308; d <= 1e308; d *= 10.0) {
        std::cout << std::left << std::setw(20) << d;

        int length;
        
        length = dtoa(out, d);
        out[length] = 0;
        std::cout << std::left << std::setw(20) << out;

        std::cout << std::endl;
    }

    delete[] out;
}

void testLog2ToLog10Approximation() {
    for (int i = 0; i <= 64; ++i) {
        int floor_log2 = i;
        int approximation = (floor_log2 * 77) >> 8;
        int knownLog10 = log10(exp2(i));

        if (approximation != knownLog10) {
            std::cout << "2^" << i << ": log10 approximation: " << approximation << ", known: " << knownLog10 << std::endl;
        }
    }
}

void testMangitudeApproximationForDoubles() {
    int col = 0;

    for (int i = 1; i <= 2046; ++i) {
        int approximation = (((unsigned)i * 1262611 + 194579) >> 22) - 308;
        // int approximation = floor((i - 1023) * log10(2.0)); //known to work

        int knownLog10 = floor(log10(exp2(i - 1023)));

        if (approximation != knownLog10) {
            std::cout << std::setw(4) << std::left << (signed)(i) << " -> " << std::setw(4) << approximation << " error " << (approximation - knownLog10);

            if (col == 4) {
                std::cout << std::endl;
                col = 0;
            } else {
                std::cout << "\t\t";
                ++col;
            }
        }
    }

    if (col != 0) {
        std::cout << std::endl;
    }

    // for (unsigned i = 0; i <= 22; ++i) {
    //     unsigned approximation = floor(log10(2.0) * (1 << i));
    //     double error = approximation / exp2(i) - log10(2.0);
    //     std::cout << "* " << std::setw(6) << approximation << " >> " << i << "\t\terror: " << error << std::endl;
    // }
}


void testLengthOfLong() {
    int length = lengthOfLong(0);
    std::cout << "length(0) == " << length << std::endl;

    unsigned long l = 1;
    for (int i = 0; i < 64; ++i) {
        int length = lengthOfLong(l);
        std::cout << "length(" << l << ") == " << length << std::endl;
        l *= 2;
    }

    l = 1;
    for (int i = 0; i < 19; ++i) {
        int length = lengthOfLong(l);
        std::cout << "length(" << l << ") == " << length << std::endl;
        l *= 10;
    }
}