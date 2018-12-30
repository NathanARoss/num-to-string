#include <math.h> //used just for INFINITY and NAN
#include <cstring>

#include "conversion.h"
#include <iostream>

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

double getMagnitude(int exponent) {
    double result = 1.0;
    double base = 10.0;

    if (exponent < 0) {
        base = 0.1;
        exponent = -exponent;
    }

    for (;;)
    {
        if (exponent & 1) {
            result *= base;
        }
        exponent >>= 1;
        if (exponent == 0) {
            break;
        }
        base *= base;
    }

    return result;
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

    int exponent = ((bits & 0x7FF0000000000000) >> 52) - 1023;
    unsigned long fraction = (bits & 0x000FFFFFFFFFFFFF) | 0x0070000000000000;

    int order = ceil(exponent * log10(2.0));
    double magnitude = getMagnitude(order);

    if (magnitude > num) {
        magnitude *= 0.1;
        --order;
    }
    
    int realOrder = (int)log10(num);
    if (order != realOrder) {
        std::cout << "order estimation failed. " << order << " != " << realOrder << std::endl;
    }

    // std::cout << num << " is 2^" << exponent << " mangitude " << magnitude << std::endl;

    // for (; order >= 0; --order) {
    //     double digit = floor(num / magnitude);
    //     num -= digit * magnitude;
    //     chars[length++] = (char)digit + '0';
    //     magnitude *= 0.1;
    // }

    strcpy(chars, "1e");
    length += 2;
    length += ltoa(order, chars + 2);


    return length;
}