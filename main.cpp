#include <iostream>

int ltoa(long num, char* chars);
int dtoa(double num, char* chars);

int main() {
    char* out = new char[256];

    long i = 0xFFFFFFFFFF000000L;
    double d = 100.12;
    
    int length = ltoa(i, out);
    std::cout << "length: " << length << std::endl;
    out[length] = 0;

    std::cout << "long" << std::endl;
    std::cout << "std: " <<  i << std::endl;
    std::cout << "me:  " << out << std::endl;

    length =  dtoa(d, out);
    out[length] = 0;

    std::cout << std::endl;
    std::cout << "double" << std::endl;
    std::cout << "std: " <<  d << std::endl;
    std::cout << "me:  " << out << std::endl;
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
    
    unsigned long mag = 1;
    do {
        mag *= 10;
        ++length;
    } while (mag <= number);

    char *p = chars + length - 1;
    do {
        unsigned long temp = number / 10;
        *p = '0' + (number - temp * 10);
        --p;
        number = temp;
    } while (number != 0);

    return length;
}

int dtoa(double num, char* chars) {
    return 0;
}