#include <iostream>
#include <iomanip>

#include "conversion.h"

void printDoubleComponents(double f);
void testltoa(long l);
void testdtoa();

void testMagnitudes();

int main() {
    //testltoa(123456789123456789);
    //testltoa(0x8000000000000000);

    // testdtoa();
    testMagnitudes();
}


void testltoa(long l) {
    char* out = new char[256];
    
    int length = ltoa(l, out);
    std::cout << "length: " << length << std::endl;
    out[length] = 0;

    std::cout << "long" << std::endl;
    std::cout << "std: " <<  l << std::endl;
    std::cout << "me:  " << out << std::endl;

    delete[] out;
}


void testdtoa() {
    char* out = new char[256];
    double tests[] = {1.0, 1.1, 1.01, 1.001, 1.0001, 1.00001, 1.123456};
    //double tests[] = {0.0, -0.0, INFINITY, -INFINITY, NAN};

    for (double d : tests) {
        int length =  dtoa(d, out);
        out[length] = 0;
        std::cout << "std: " << std::left << std::setw(20) << d << "me: " << out << std::endl;
    }

    delete[] out;
}


void printDoubleComponents(double f) {
    std::cout << std::left << std::setw(20) << f;

    unsigned long bits = *reinterpret_cast<unsigned long*>(&f);

    bool sign = bits >> 63;
    std::cout << (sign ? "-" : "");

    unsigned long fraction = bits & 0x000FFFFFFFFFFFFF;
    std::cout << "1.";
    for (int i = 0; i < 52; ++i) {
        char digit = ((fraction >> i) & 1) + '0';
        std::cout << digit;
    }
 
    int exponent = ((bits & 0x7FF0000000000000) >> 52) - 1023;
    std::cout << " * 2^" << exponent << std::endl;
}


void testMagnitudes() {
    char* out = new char[256];

    for (double d = 1e308; d >= 1e-308; d *= 0.1) {
        int length =  dtoa(d, out);
        out[length] = 0;
        //std::cout << "std: " << std::left << std::setw(20) << d << "me: " << out << std::endl;
    }

    delete[] out;
}