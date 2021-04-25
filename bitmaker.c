#include "bitmaker.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void finishByte(bitmaker * b)
{
    if(b->bytecount == b->bytecapacity)
    {
        b->bytecapacity = b->bytecapacity + b->bytecapacity / 2;
        if(b->bytecapacity == 0)
            b->bytecapacity = 8;

        unsigned char * newbytes = realloc(b->bytes, b->bytecapacity);
        if(!newbytes)
            abort();

        b->bytes = newbytes;
    }

    b->bytes[b->bytecount++] = b->buff;
}

void bitmaker_init(bitmaker * b)
{
    memset(b, 0x0, sizeof(bitmaker));
}

int bitmaker_addBit(bitmaker * b, int v)
{
    b->buff |= v << (7 - b->count);
    b->count += 1;

    if(b->count == 8)
    {
        finishByte(b);
        b->count = 0;
        b->buff = 0u;
    }

    return b->count + b->bytecount * 8 - 1;
}

void bitmaker_addBigBytes(bitmaker * b, int size, unsigned value)
{
    while(size)
    {
        const int bit = 1 & (value >> (size - 1));
        bitmaker_addBit(b, bit);
        --size;
    }
}

void bitmaker_padToByte(bitmaker * b)
{
    while(b->count != 0)
        bitmaker_addBit(b, 0);
}

void bitmaker_setBitAtAnchor(bitmaker * b, int anchor, int v)
{
    if(anchor < 0)
        return;

    const int idx = anchor / 8;

    if(idx == b->bytecount)
    {
        int shift = 7 - anchor % 8;
        b->buff |= 1 << shift;
        b->buff ^= ((!v) << shift);
    }

    if(idx < b->bytecount)
    {
        int shift = 7 - anchor % 8;
        b->bytes[idx] |= 1 << shift;
        b->bytes[idx] ^= ((!v) << shift);
    }
}
