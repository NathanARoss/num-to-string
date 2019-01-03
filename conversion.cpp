#include <math.h> //used just for INFINITY and NAN
#include <cstring>

#include <iostream>
#include <iomanip>
#include "conversion.h"

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



int lengthOfLong(unsigned long num) {
    static const unsigned long magnitudes[] = {
        10L, //1
        100L, //2
        10000L, //4
        100000000L, //8
    };

    int length = 1;
    unsigned long pow10 = 1;

    int i = 3;
    do {
        unsigned long temp = pow10 * magnitudes[i];
        if (temp <= num) {
            pow10 = temp;
            length += 1 << i;

            if (i == 2 && length == 13) {
                //check 10000L twice
                continue;
            }
        }

        --i;
    } while (i >= 0);

    return length;
}


int ltoa(char* chars, long num) {
    unsigned long number;
    int length = 0;

    if (num < 0) {
        number = -num;
        chars[0] = '-';
        ++length;
    } else {
        number = num;
    }
    
    length += lengthOfLong(number);

    char *p = chars + length - 1;
    do {
        *p-- = '0' + number % 10;
        number /= 10;
    } while (number != 0);

    return length;
}

int dtoa(char* chars, double num) {
    char* c = chars;
    unsigned long bits = *reinterpret_cast<unsigned long*>(&num);

    if ((signed long) bits < 0) {
        *c++ = '-';
        num = -num;
    }

    if (num != num) {
        strcpy(c, "nan");
        c += 3;
    }
    else if (num == INFINITY) {
        strcpy(c, "inf");
        c += 3;
    }
    else if (bits << 1 == 0) {
        *c++ = '0';
    }
    else {
        int floor_log2 = (bits >> 52 & 0x7FF) - 1023;
        int power = ceil(floor_log2 * log10(2.0));

        //extract the power of 10 and normalize
        num = num * pow(10.0, -power) * (1.0 + 1e-14); //round the last few digits up
        if (num < 1.0) {
            num *= 10.0;
            --power;
        }

        double error = 1e-13;
        do {
            double digit = floor(num);
            num -= digit;
            *c++ = (char)digit + '0';

            error *= 10.0;
            num *= 10.0;
        } while (num > error);

        *c++ = 'e';
        c += ltoa(c, power);
    }

    return c - chars;
}