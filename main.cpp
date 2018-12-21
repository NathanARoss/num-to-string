#include <iostream>
#include <math.h> //used just for INFINITY and NAN

int ltoa(long num, char* chars);
int dtoa(double num, char* chars);
void testltoa(long l);
void testdtoa(double d);

int main() {
    //testltoa(123456789123456789);
    //testltoa(0x8000000000000000);

    testdtoa(INFINITY);
    testdtoa(-INFINITY);
    testdtoa(NAN);
    testdtoa(0.0);
    testdtoa(-0.0);
    testdtoa(1.0);
    testdtoa(3.14159);
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
    if (bits & 0x8000000000000000 != 0) {
        chars[0] = '-';
        ++length;
    }

    unsigned exponent = ((bits & 0x7FF0000000000000) >> 52) - 1023;
    unsigned fraction = bits & 0x000FFFFFFFFFFFFF;

    //TODO
    chars[length++] = '1';
    chars[length++] = '.';
    for (int i = 51; i >= 0; --i) {
        chars[length + i] = '0' + ((fraction >> i) & 1);
    }
    length += 52;

    chars[length++] = ' ';
    chars[length++] = '*';
    chars[length++] = ' ';
    chars[length++] = '2';
    chars[length++] = '^';
    length += ltoa(exponent, chars + length);     

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

void testdtoa(double d) {
    char* out = new char[256];

    int length =  dtoa(d, out);
    out[length] = 0;
    std::cout << "std: " <<  d << std::endl;
    std::cout << "me:  " << out << std::endl;
    std::cout << std::endl;

    delete[] out;
}