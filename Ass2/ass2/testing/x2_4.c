// Test tsig ADT

#include <stdio.h>
#include "defs.h"
#include "tuple.h"
#include "psig.h"
#include "bits.h"
#include "reln.h"

int main(void)
{
	Reln r = openRelation("R");
	Tuple t = readTuple(r, stdin);
	Bits b = makePageSig(r, t);
	showBits(b); putchar('\n');
	int n0, n1, tot; n0 = n1 = tot = 0;
	for (unsigned int i = 0; i < psigBits(r); i++) {
		if (bitIsSet(b,i)) n1++; else n0++;
		tot++;
	}
	printf("%d total, %d set, %d zero\n", tot, n1, n0);
	return 0;
}
