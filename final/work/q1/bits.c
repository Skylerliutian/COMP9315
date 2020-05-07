// bits.c ... functions on bit-strings
// part of Multi-attribute Linear-hashed Files
// Bit-strings are 32-bit unsigned quantities
// Last modified by John Shepherd, July 2019

#include <assert.h>
#include "bits.h"

// check if the bit at position is 1

int bitIsSet(Bits val, int position)
{
    assert(0 <= position && position <= 31);
    Bits mask = (1 << position);
    return ((val & mask) != 0);
}

// set the bit at position to 1

Bits setBit(Bits val, int position)
{
    assert(0 <= position && position <= 31);
    Bits mask = (1 << position);
    return (val | mask);
}

// set the bit at position to 0

Bits unsetBit(Bits val, int position)
{
    assert(0 <= position && position <= 31);
    Bits mask = (~(1 << position));
    return (val & mask);
}

// extract just lower-order n bits

Bits getLower(Bits b, int n)
{
    assert(1 <= n && n <= 32);
    int i; Bits mask = 0;
    for (i = 0; i < n; i++) mask |= (1<<i);
    return b&mask;
}

// convert 32-bit unsigned quantity to string
// place in a user-supplied buffer of length > 36

void bitsString(Bits val, char *buf)
{
    int i,j; char ch;
    Bits bit = 0x80000000;

    i = j = 0;
    while (bit != 0) {
        ch = ((val & bit) != 0) ? '1' : '0';
        buf[i++] = ch;
        j++;
        if (j%8 == 0) buf[i++] = ' ';
        bit = bit >> 1;
    }
    buf[--i] = '\0';
}
