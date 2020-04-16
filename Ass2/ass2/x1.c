// Test Bits ADT

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv)
{
	Bits b = newBits(60);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 5);
	printf("t=1: "); showBits(b); printf("\n");
	setBit(b, 0);
	setBit(b, 50);
	setBit(b, 59);
	printf("t=2: "); showBits(b); printf("\n");
	if (bitIsSet(b,5)) printf("Bit 5 is set\n");
	if (bitIsSet(b,10)) printf("Bit 10 is set\n");
	printf("t=3: "); showBits(b); printf("\n");
	unsetBit(b, 40);
	printf("t=4: "); showBits(b); printf("\n");
	if (bitIsSet(b,20)) printf("Bit 20 is set\n");
	if (bitIsSet(b,40)) printf("Bit 40 is set\n");
	if (bitIsSet(b,50)) printf("Bit 50 is set\n");
	setBit(b, 59);


	setAllBits(b);
	printf("t=3: "); showBits(b); printf("\n");
	unsetAllBits(b);
	printf("t=4: "); showBits(b); printf("\n");

	// Bits b1 = newBits(16);
	// Bits b2 = newBits(15);
	// setBit(b1, 14);
	// setBit(b1, 5);
	// setBit(b1, 2);
	// setBit(b1, 3);
	// setBit(b2, 2);
	// setBit(b2, 3);
	// printf("b1: "); showBits(b1); printf("\n");
	// printf("b2: "); showBits(b2); printf("\n");
	// andBits(b1, b2);
	// printf("b1: "); showBits(b1); printf("\n");
	// andBits(b2, b1);
	// printf("b2: "); showBits(b2); printf("\n");
	// orBits(b1, b2);
	// printf("b1: "); showBits(b1); printf("\n");
	// orBits(b2, b1);
	// printf("b2: "); showBits(b2); printf("\n");
	// if (isSubset(b1,b2)) printf("b12yes\n");
	// else printf("b12no\n");
	// if (isSubset(b2,b1)) printf("b21yes\n");
	// else printf("b21no\n");
	// freeBits(b1);
	return 0;
}
