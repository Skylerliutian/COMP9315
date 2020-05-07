#include "bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBITS 32
#define NCHARS (NBITS+1)

int main(int argc, char **argv)
{
	uint32 hashVal, less;
	int    n;
	char   a[NCHARS], b[NCHARS];

	if (argc < 2) {
		fprintf(stderr,"Usage: %s keyVal nbits\n",argv[0]);
		exit(1);
	}
	n = (argc < 3) ? 32 : atoi(argv[2]);
	if (n == 0) n = 32;

	printf("key  = %s\n", argv[1]);
	hashVal = hash_any((unsigned char *)argv[1], strlen(argv[1]));
	printf("hash = %s\n", showBits(hashVal,a));
	less = bits(n, hashVal);
	printf("bits = %s\n", showBits(less,b));
	exit(0);
}
