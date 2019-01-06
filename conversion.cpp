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
        1ULL,
        10ULL,
        100ULL,
        1000ULL,
        10000ULL,
        100000ULL,
        1000000ULL,
        10000000ULL,
        100000000ULL,
        1000000000ULL,
        10000000000ULL,
        100000000000ULL,
        1000000000000ULL,
        10000000000000ULL,
        100000000000000ULL,
        1000000000000000ULL,
        10000000000000000ULL,
        100000000000000000ULL,
        1000000000000000000ULL,
        10000000000000000000ULL,
    };

    int floor_log2;
    if (num == 0) {
        floor_log2 = 4;
    } else {
        floor_log2 = 64 - __builtin_clzl(num);
    }

    int floor_log10 = (floor_log2 * 77) >> 8; // * log10(2.0);
    return floor_log10 + (num >= magnitudes[floor_log10]);
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

int dtoa(char* chars, double num, int requestedDigits) {
    char* c = chars;
    const unsigned long bits = *reinterpret_cast<long*>(&num);

    //normal, denormal, and special cases can all be negated
    if ((long) bits < 0) {
        *c++ = '-';
        num = -num;
    }

    //special cases
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
        double acceptableError;
        if (requestedDigits > 0) {
            //only the requested digits are significant.
            acceptableError = pow(10.0, -requestedDigits);
            
            //round beyond requested digits
            num += acceptableError * (0.5 + 1e-10);
        } else {
            //only the first 12 digits are significant
            acceptableError = 1e-12;
            
            //round beyond signficant digits
            num *= 1.0 + 1e-13;
        }

        //approximate power of 10 from power of 2 encoded within the double
        const unsigned exponent2 = bits >> 52 & 0x7FF;
        //approximation for: floor((exponent2 - 1023) * log10(2.0));  handles 11 bit uints
        int power = ((exponent2 * 1262611 + 194579) >> 22) - 308;

        //adjust approximation and normalize argument for printing
        double scaledNum = num * pow(10.0, -power);
        if (scaledNum > 10.0) {
            scaledNum *= 0.1;
            ++power;
        }

        //detect fixed point formatting, either forced or automatic
        int digitsAfterDot = 0;
        if (power > -5 && power < 6 || requestedDigits >= 0) {
            if (power < 0) {
                //return the number to its original scale
                scaledNum = num;
            } else {
                //delay placement of dot
                digitsAfterDot = -power;
            }

            //hide scientific notation and avoid generating one too few digits
            if (requestedDigits < 0) {
                requestedDigits = 0;
            }
        }

        //generate all significant digits
        do {
            if (digitsAfterDot == 1) {
                *c++ = '.';
            }

            double digit = floor(scaledNum);
            scaledNum -= digit;
            *c++ = (char)digit + '0';

            acceptableError *= 10.0;
            scaledNum *= 10.0;

            ++digitsAfterDot;
        } while (scaledNum > acceptableError || digitsAfterDot <= requestedDigits);

        //print power of 10 if not using fixed point formatting
        if (requestedDigits < 0) {
            *c++ = 'e';

            if (power < 0) {
                *c++ = '-';
                power = -power;
            }

            //generate digits backwards
            char *cc = c;

            do {
                *c++ = '0' + power % 10;
                power /= 10;
            } while (power != 0);

            //swap first and last digit
            int temp = *(c-1);
            *(c-1) = *cc;
            *cc = temp;
        }
    }

    return c - chars;
}