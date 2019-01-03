#include <iostream>
#include <iomanip>
#include <math.h>
#include <limits>

#include "conversion.h"

void printDoubleComponents(double f);
void testltoa(long l);
void testdtoa();

void testMagnitudes();

int main() {
    //testltoa(123456789123456789);
    //testltoa(0x8000000000000000);

    testdtoa();
    // testMagnitudes();
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