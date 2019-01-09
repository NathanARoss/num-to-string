#include <math.h> //used just for INFINITY and NAN
#include <cstring>

#include <iostream>
#include <iomanip>
#include "conversion.h"

//calculate length of a long using a LUT instead of a loop
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
        floor_log2 = 64 - __builtin_clzl(num); //floor(log2(num))
    }

    int floor_log10 = (floor_log2 * 77) >> 8; // * log10(2.0);
    return floor_log10 + (num >= magnitudes[floor_log10]);
}


char* ltoa(char* chars, long num) {
    unsigned long number;

    if (num < 0) {
        number = -num;
        *chars++ = '-';
    } else {
        number = num;
    }
    
    // chars += lengthOfLong(number);

    unsigned long copy = number;
    do {
        ++chars;
        copy /= 10;
    } while (copy != 0);

    char* next = chars;
    do {
        *--chars = '0' + number % 10;
        number /= 10;
    } while (number != 0);

    return next;
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


double pow10(int exponent) {
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

char* dtoa(char* chars, double num, int requestedDigits) {
    const unsigned long bits = *reinterpret_cast<long*>(&num);

    //normal, denormal, and special cases can all be negated
    if ((long) bits < 0) {
        *chars++ = '-';
        num = -num;
    }

    //special cases
    if (num != num) {
        strcpy(chars, "nan");
        chars += 3;
    }
    else if (num == INFINITY) {
        strcpy(chars, "inf");
        chars += 3;
    }
    else if (bits << 1 == 0) {
        *chars++ = '0';
    }
    else {
        //approximate power of 10 from power of 2 encoded within the double
        const unsigned exponent2 = bits >> 52 & 0x7FF;
        //approximation for: floor((exponent2 - 1023) * log10(2.0));  handles 11 bit uints
        int power = ((exponent2 * 78913 + 12353) >> 18) - 308;

        //preform approximate normalization
        // double inverseMagnitude = pow(10.0, -power);
        double inverseMagnitude = pow10(-power);

        double acceptableError;
        if (requestedDigits > 0) {
            //only the requested digits are significant.
            //only a handful of digits will be requested, so a small loop works
            acceptableError = 1;
            int i = 0;
            do {
                acceptableError *= 0.1;
            } while (++i < requestedDigits);
            
            //round beyond requested digits
            num += acceptableError * (0.5 + 1e-10);
        } else {
            //only the first 12 digits are significant
            acceptableError = 1e-12;

            //round the scaling factor itself to avoid overflowing num
            inverseMagnitude *= 1.0 + 1e-14;
        }

        //adjust approximate normalization
        double scaledNum = num * inverseMagnitude;
        if (scaledNum > 10.0) {
            scaledNum *= 0.1;
            ++power;
        }

        //detect fixed point formatting
        if (power > -5 && power < 6 && requestedDigits < 0) {
            //hide scientific notation and avoid generating one too few digits
            requestedDigits = 0;
        }

        //handle fixed point formatting
        int digitsAfterDot = 0;
        if (requestedDigits >= 0) {   
            if (power < 0) {
                //return the number to its original scale after rounding
                scaledNum = num * (1.0 + 1e-13);
            } else {
                //delay placement of dot
                digitsAfterDot = -power;
            }
        }

        //generate all significant digits
        do {
            if (digitsAfterDot == 1) {
                *chars++ = '.';
            }

            double digit = floor(scaledNum);
            scaledNum -= digit;
            *chars++ = (char)digit + '0';

            acceptableError *= 10.0;
            scaledNum *= 10.0;

            ++digitsAfterDot;
        } while (scaledNum > acceptableError || digitsAfterDot <= requestedDigits);

        //print power of 10 if not using fixed point formatting
        if (requestedDigits < 0) {
            *chars++ = 'e';

            if (power < 0) {
                *chars++ = '-';
                power = -power;
            }

            //generate digits backwards
            char *cc = chars;

            do {
                *chars++ = '0' + power % 10;
                power /= 10;
            } while (power != 0);

            //swap first and last digit
            int temp = *(chars-1);
            *(chars-1) = *cc;
            *cc = temp;
        }
    }

    return chars;
}