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


int ltoa(long num, char* chars) {
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

int dtoa(double num, char* chars) {
    if (num != num) {
        strcpy(chars, "nan");
        return 3;
    }
    if (num == INFINITY) {
        strcpy(chars, "inf");
        return 3;
    }
    if (num == -INFINITY) {
        strcpy(chars, "-inf");
        return 4;
    }

    unsigned long bits = *reinterpret_cast<unsigned long*>(&num);
    if (bits == 0) {
        chars[0] = '0';
        return 1;
    }
    if (bits == 0x8000000000000000) {
        strcpy(chars, "-0");
        return 2;
    }

    int length = 0;
    if (bits >> 63 == 1) {
        chars[0] = '-';
        ++length;
        num = -num;
    }

    int floor_log2 = ((bits & 0x7FF0000000000000) >> 52) - 1023;
    int power = ceil(floor_log2 * log10(2.0));
    if (pow(10.0, power) > num) {
        --power;
    }
    
    if (power != floor(log10(num))) {
        std::cout << "power estimation failed. log10(" << num << ") != " << power << std::endl;
        return 0;
    }

    const double scale = pow(10.0, -power);

    double before = std::nextafter(num * scale, -INFINITY);
    num = std::nextafter(num * scale, INFINITY);
    double epsilon = num - before;

    while (num > epsilon) {
        double digit = floor(num);
        num -= digit;
        chars[length++] = (char)digit + '0';

        num *= 10.0;
        epsilon *= 10.0;
    }

    return length;
}