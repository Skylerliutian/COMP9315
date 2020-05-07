#include "bits.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	int w = 0x00000000;
	int x = 0xFFFFFFFF;
	int y = 0x55555555;
	int z = 0x12345678;
	char b[100];

	printf("w = 0x%08x = %s\n", w, showBits(w,b));
	printf("x = 0x%08x = %s\n", x, showBits(x,b));
	printf("y = 0x%08x = %s\n", y, showBits(y,b));
	printf("z = 0x%08x = %s\n", z, showBits(z,b));

	return 0;
}
