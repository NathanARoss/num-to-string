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
void testPow10();

int main() {
    // std::cout << "ltoa:" << std::endl;
    // testltoa(123456789123456789);
    // testltoa(0x8000000000000000);

    testMagnitudes();
    std::cout << std::endl;
    testdtoa();

    // testLog2ToLog10Approximation();
    // testLengthOfLong();
    // testMangitudeApproximationForDoubles();

    // testPow10();
}


void testltoa(long l) {
    char* out = new char[256];
    
    int length = ltoa(out, l);
    out[length] = 0;

    std::cout << std::setw(20) << std::left << l << "\t" << out << std::endl;

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
        std::numeric_limits<double>::min(),
        // std::numeric_limits<double>::max(),
        // 1.796e308,
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
    int canBeRaised = 0x7FFFFFFF;
    int needsToBeRaised = 0;
    int canBeLowered = 0x7FFFFFFF;
    int needsToBeLowered = 0;

    for (int i = 0; i <= 64; ++i) {
        int knownLog10 = log10(exp2(i));
        int lowBound = knownLog10 << 8;
        int highBound = lowBound + (1 << 8) - 1;

        int approximation = i * 77;

        needsToBeRaised = std::max(needsToBeRaised, lowBound - approximation);
        needsToBeLowered = std::max(needsToBeLowered, approximation - highBound);
        canBeLowered = std::min(canBeLowered, std::max(approximation - lowBound, 0));
        canBeRaised = std::min(canBeRaised, std::max(highBound - approximation, 0));
    }

    std::cout << "needs to be raised: " << needsToBeRaised << std::endl;
    std::cout << "can be raised: " << canBeRaised << std::endl;
    std::cout << "needs to be lowered: " << needsToBeLowered << std::endl;
    std::cout << "can be lowered: " << canBeLowered << std::endl;
}

void testMangitudeApproximationForDoubles() {
    int canBeRaised = 0x7FFFFFFF;
    int needsToBeRaised = 0;
    int canBeLowered = 0x7FFFFFFF;
    int needsToBeLowered = 0;

    for (int i = 1; i <= 2046; ++i) {
        int knownLog10 = floor(log10(exp2(i - 1023))) + 308;
        int lowBound = knownLog10 << 18;
        int highBound = lowBound + (1 << 18) - 1;

        // int approximation = floor((i - 1023) * log10(2.0)); //known to work
        int approximation = (unsigned)i * 78913 + 12353; // + 218 would not change results

        needsToBeRaised = std::max(needsToBeRaised, lowBound - approximation);
        needsToBeLowered = std::max(needsToBeLowered, approximation - highBound);
        canBeLowered = std::min(canBeLowered, std::max(approximation - lowBound, 0));
        canBeRaised = std::min(canBeRaised, std::max(highBound - approximation, 0));
    }

    std::cout << "needs to be raised: " << needsToBeRaised << std::endl;
    std::cout << "can be raised: " << canBeRaised << std::endl;
    std::cout << "needs to be lowered: " << needsToBeLowered << std::endl;
    std::cout << "can be lowered: " << canBeLowered << std::endl;

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


void testPow10() {
    for (int i = -308; i <= 308; ++i) {
        double approximation = pow10(i);
        double known = pow(10.0, i);

        if (approximation != known) {
            std::cout << "10^" << std::setw(4) << std::left << i << ": ";
            // std::cout << "known : ";
            // printDoubleComponents(known);
            // std::cout << "approx: ";
            // printDoubleComponents(approximation);
            std::cout << "error: " << (approximation - known) / known << std::endl;
            // std::cout << std::endl;
        }
    }
}