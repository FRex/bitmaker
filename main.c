#include <stdio.h>
#include "bitmaker.h"

void printByte(unsigned char b)
{
    for(int i = 0; i < 8; ++i)
    {
        const int bit = 1 & (b >> (7 - i));
        fputc('0' + bit, stdout);
    }
}

int main(int argc, char ** argv)
{
    bitmaker myb;
    bitmaker * b = &myb;
    bitmaker_init(b);

    bitmaker_addBit(b, 1);
    bitmaker_addBit(b, 1);
    int anchor = bitmaker_addBit(b, 1);
    bitmaker_addBit(b, 1);




    bitmaker_padToByte(b);
    bitmaker_setBitAtAnchor(b, anchor, argc > 1);



    printf("anchor is %d and setting it to %d\n", anchor, argc > 1);


    bitmaker_padToByte(b);


    for(int i = 0; i < b->bytecount; ++i)
    {
        printByte(b->bytes[i]);
        putc(' ', stdout);
    }

    puts("");

    for(int i = 0; i < b->bytecount; ++i)
    {
        printf("%02x       ", b->bytes[i]);
    }

    puts("");
}
