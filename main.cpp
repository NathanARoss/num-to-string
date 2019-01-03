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
    // double tests[] = {0.696, 696.0 / 1000.0};
    double tests[] = {0.0, -0.0, INFINITY, -INFINITY, NAN, -NAN, 696.0 / 1000.0};

    // for (double d = 0; d <= 1000.0; d += 1.0) {
    for (double d : tests) {
        int length =  dtoa(out, d);
        out[length] = 0;
        std::cout << std::left << std::setw(20) << d << out << std::endl;
    }

    delete[] out;
}


void testMagnitudes() {
    char* out = new char[256];

    for (double d = 1e-308; d <= 1e308; d *= 1.1) {
        int length = dtoa(out, d);
        if (length == -1) {
            break;
        }
        out[length] = 0;
        std::cout << std::left << std::setw(20) << d << out << std::endl;
    }

    delete[] out;
}