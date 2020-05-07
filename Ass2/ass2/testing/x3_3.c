// Test tsig ADTs

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "tsig.h"
#include "bits.h"

int main(void)
{
	Reln r = openRelation("R");
	Tuple t = readTuple(r, stdin);
	Bits b = makeTupleSig(r, t);
	showBits(b); putchar('\n');
	Tuple q = readTuple(r, stdin);
	Bits bq = makeTupleSig(r, q);
	showBits(bq); putchar('\n');
	if (isSubset(bq,b))
		printf("Matches\n");
	else
		printf("No match\n");
	return 0;
}
