#include <iostream>
#include <iomanip>
#include <math.h> //used just for INFINITY and NAN

int ltoa(long num, char* chars);
int dtoa(double num, char* chars);
void testltoa(long l);
void testdtoa();
double getWholeDigits(int exponent);

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

double getWholeDigits(int exponent) {
    double result = 1.0;
    double base = 10.0;

    if (exponent < 0) {
        return 1.0;
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
        chars[0] = 'n';
        chars[1] = 'a';
        chars[2] = 'n';
        return 3;
    }
    if (num == INFINITY) {
        chars[0] = 'i';
        chars[1] = 'n';
        chars[2] = 'f';
        return 3;
    }
    if (num == -INFINITY) {
        chars[0] = '-';
        chars[1] = 'i';
        chars[2] = 'n';
        chars[3] = 'f';
        return 4;
    }

    unsigned long bits = *reinterpret_cast<unsigned long*>(&num);
    if (bits == 0) {
        chars[0] = '0';
        return 1;
    }
    if (bits == 0x8000000000000000) {
        chars[0] = '-';
        chars[1] = '0';
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

    int order = ((double)exponent * log10(2.0));
    double magnitude = getWholeDigits(order);
    std::cout << num << " is 2^" << exponent << " mangitude " << magnitude;

    if (magnitude * 10.0 <= num) {
        magnitude *= 10.0;
        ++order;
        std::cout << "++";
    }
    
    std::cout << std::endl;

    // for (; order > 0; --order) {
    //     double digit = floor(num / magnitude);
    //     chars[length++] = (char)digit + '0';
    //     num -= digit * magnitude;
    //     magnitude *= 0.1;
    // }


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
    double tests[] = {0.0, 1.0, 2.0, 10.0, 11.0, 125.0, 11123456.0, 100000000000.0};

    for (double d = 1e-308; d <= 1e308; d *= 10.0) {
        int length =  dtoa(d, out);
        out[length] = 0;
        //std::cout << "std: " << std::fixed << std::left << std::setw(20) << d << "me:  " << out << std::endl;
    }

    delete[] out;
}

void printDoubleComponents(double f) {
    unsigned long bits = *reinterpret_cast<unsigned long*>(&f);

    bool sign = bits >> 63;
    unsigned long exponent = (bits & 0x7FF0000000000000) >> 52;
    unsigned long fraction = bits & 0x000FFFFFFFFFFFFF;


    std::cout << (sign ? "-" : "") << fraction << " * 2^" << exponent << std::endl;
}