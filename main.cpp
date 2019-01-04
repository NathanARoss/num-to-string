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
void testLengthOfLong();

int main() {
    //testltoa(123456789123456789);
    //testltoa(0x8000000000000000);

    // testdtoa();
    // testMagnitudes();
    // testLog2ToLog10Approximation();
    testLengthOfLong();
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

        static constexpr unsigned log10_from_log2[65] =
        {
            0 ,0 ,0 ,0 , 1 ,1 ,1 , 2 ,2 ,2 ,
            3 ,3 ,3 ,3 , 4 ,4 ,4 , 5 ,5 ,5 ,
            6 ,6 ,6 ,6 , 7 ,7 ,7 , 8 ,8 ,8 ,
            9 ,9 ,9 ,9 , 10,10,10, 11,11,11,
            12,12,12,12, 13,13,13, 14,14,14,
            15,15,15,15, 16,16,16, 17,17,17,
            18,18,18,18, 19
        };
        int knownLog10 = log10_from_log2[i];

        std::cout << "2^" << i << ": log10 approximation: " << approximation << ", known: " << knownLog10;

        if (approximation != knownLog10) {
            std::cout << " failed";
        }
        std::cout << std::endl;
    }
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