// bits.h ... interface to functions on bit-strings
// part of Multi-attribute Linear-hashed Files
// See bits.c for details of functions
// Last modified by John Shepherd, July 2019

#ifndef BITS_H
#define BITS_H 1

typedef unsigned int Bits;

int bitIsSet(Bits, int);
Bits setBit(Bits, int);
Bits unsetBit(Bits, int);
Bits getLower(Bits, int);
void bitsString(Bits, char *);

#endif
