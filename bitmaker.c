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
        unsigned char * newcolors = realloc(b->colors, b->bytecapacity);
        if(!newbytes || !newcolors)
            abort();

        b->bytes = newbytes;
        b->colors = newcolors;
    }

    b->bytes[b->bytecount] = b->buff;
    b->colors[b->bytecount] = b->colorbuff;

    ++b->bytecount;
}

void bitmaker_init(bitmaker * b)
{
    memset(b, 0x0, sizeof(bitmaker));
}

static int internal_addBit(bitmaker * b, int v, int color)
{
    b->buff |= v << (7 - b->count);
    b->colorbuff |= color << (7 - b->count);
    b->count += 1;

    if(b->count == 8)
    {
        finishByte(b);
        b->count = 0;
        b->buff = 0u;
        b->colorbuff = 0u;
    }

    return b->count + b->bytecount * 8 - 1;
}

int bitmaker_addBit(bitmaker * b, int v)
{
    b->curcolor = !b->curcolor;
    return internal_addBit(b, v, b->curcolor);
}

void bitmaker_addBigBytes(bitmaker * b, int size, unsigned value)
{
    b->curcolor = !b->curcolor;
    while(size)
    {
        const int bit = 1 & (value >> (size - 1));
        internal_addBit(b, bit, b->curcolor);
        --size;
    }
}

void bitmaker_padToByte(bitmaker * b)
{
    b->curcolor = !b->curcolor;
    while(b->count != 0)
        internal_addBit(b, 0, b->curcolor);
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
