#include <iostream>
#include <iomanip>
#include <math.h> //used just for INFINITY and NAN
#include <cstring>

int ltoa(long num, char* chars);
int dtoa(double num, char* chars);
void testltoa(long l);
void testdtoa();
double getMagnitude(int exponent);
void printDoubleComponents(double f);

int main() {
    //testltoa(123456789123456789);
    //testltoa(0x8000000000000000);

    testdtoa();
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
    
    // std::cout << num << " is 2^" << exponent << " mangitude " << magnitude << std::endl;

    for (; order >= 0; --order) {
        double digit = floor(num / magnitude);
        num -= digit * magnitude;
        chars[length++] = (char)digit + '0';
        magnitude *= 0.1;
    }


    return length;
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
    // double tests[] = {1.0, 1.1, 1.01, 1.001, 1.0001, 1.00001, 1.123456};
    double tests[] = {0.0, -0.0, INFINITY, -INFINITY, NAN};

    for (double d : tests) {
        int length =  dtoa(d, out);
        out[length] = 0;
        std::cout << "std: " << std::fixed << std::left << std::setw(16) << d << "me: " << out << std::endl;
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