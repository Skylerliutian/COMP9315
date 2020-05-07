// Extract d bits from uint32
// if d +ve, get low-order bits
// if d -ve, get high-order bits

#include <stdio.h>
#include <assert.h>
#include "bits.h"

uint32 bits(int d, uint32 val)
{
	assert(d >= -32 && d <= 32);
	uint32 mask;
	char buf[33];

	if (d > 0) {
		mask = (0x1 << d) - 1;
		printf("Mask = %s\n",showBits(mask,buf));
		return (val & mask);
	}
	else if (d < 0) {
		d = 0-d;
		mask = ((0x1 << d) - 1) << (32-d);
		printf("Mask = %s\n",showBits(mask,buf));
		return ((val & mask) >> (32-d));
	}
	else
		return 0;
}
