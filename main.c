#include <stdio.h>
#include "bitmaker.h"

void printByte(unsigned char b, unsigned char colorbyte)
{
    for(int i = 0; i < 8; ++i)
    {
        const int bit = 1 & (b >> (7 - i));
        const int col = 1 & (colorbyte >> (7 - i));
        if(col)
            fputs("\e[38;2;255;0;0m", stdout);
        else
            fputs("\e[38;2;0;255;0m", stdout);

        fputc('0' + bit, stdout);
        fputs("\e[0m", stdout);
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
        printByte(b->bytes[i], b->colors[i]);
        putc(' ', stdout);
    }

    puts("");

    for(int i = 0; i < b->bytecount; ++i)
    {
        printf("%02x       ", b->bytes[i], b->colors[i]);
    }

    puts("");
}
