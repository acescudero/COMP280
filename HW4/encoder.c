/*
Program that encodes a height into an ARINC-429 Message
Author: Augusto Escudero
*/

#include <stdio.h>

struct ArincData {
    unsigned int parity: 1;
    unsigned int sign: 2;
    unsigned int dataWhole: 19;
    unsigned int sdi: 2;
    unsigned int label: 8;
};

int countBits(int a) {

    int count = 0;
    while (a>0) {
        count += a & 1;
        a >>= 1;
    }

    return count;
}

int main(int argc, char *argv[]) {


    float val;
    sscanf(argv[1],"%f",&val);
    float fractionalPart = val-(int)val;
    int storedFractional = fractionalPart*8;

    int resultData = (val<1 && fractionalPart!=0)? val*8:((int)val << 3) | storedFractional; //If the value is negtive and has a fractional part, multiply by 8 to have it in 2's complement
    if (val<1) {
        resultData = resultData | 0x40000;
    }

    struct ArincData message = {0b0, 0b11, resultData, 0b00, 0b10110001};
    
    int firstPart = message.parity << 7 | message.sign << 5 | (message.dataWhole >> 14);
    int secondPart = (message.dataWhole >> 6) & 0xFF;
    int thirdPart = ((message.dataWhole << 2) & 0b11111100) | message.sdi;
    int fourthPart = message.label;
    int dataWholeResult = message.dataWhole;

    int bitCount = countBits(dataWholeResult) + 6;
    if (bitCount%2!=0) {
        message.parity = 0b1;
    }

    printf("%02x %02x %02x %02x", firstPart, secondPart, thirdPart, fourthPart);

    return 0;
}
