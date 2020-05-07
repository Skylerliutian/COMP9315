// bits.h ... interface to functions on bit-strings
// See bits.c for details of functions

#ifndef BITS_H
#define BITS_H 1

typedef enum {false=0, true=1} bool;
typedef unsigned int uint;

typedef struct {
	int  length; // how many bits (<=32)
	uint bits;   // the actual bits
} Bits;

bool isBits(char *);
Bits strToBits(char *);
int bitsToInt(Bits);
int bitIsSet(Bits, int);
Bits setBit(Bits, int);
Bits unsetBit(Bits, int);
Bits zeroBits(int);
int nBits(Bits);
void showBits(Bits, char *);

#endif
