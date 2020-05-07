// joinsim.c ... simulate nested loop join buffering effects

#include <stdio.h>
#include <stdlib.h>
#include "bufpool.h"

void exitUsage(char *);

// main ... process args and run simulation

int main(int argc, char **argv)
{
	int  outer, inner, nbufs;
	char strategy = 'L';
	int  status;

	// collect argv values
	if (argc < 5) { exitUsage(argv[0]); }
	sscanf(argv[1], "%d", &outer);
	sscanf(argv[2], "%d", &inner);
	sscanf(argv[3], "%d", &nbufs);
	strategy = argv[4][0];

	// initalise buffer pool
	BufPool pool = initBufPool(nbufs,strategy);

	// simulate the nested loop join
	// select * from R join S ...
	int i, j;
	for (i = 0; i < outer; i++) {
		request_page(pool,'R',i);
		for (j = 0; j < inner; j++) {
			request_page(pool,'S',j);
			// process Ri and Sj
			release_page(pool,'S',j);
		}
		release_page(pool,'R',i);
	}

	// show buffer pool usage
	showPoolUsage(pool);
}

void exitUsage(char *prog)
{
	fprintf(stderr, "Usage: %s OuterPages InnerPages Slots Strategy(M|L|C)\n", prog);
	exit(1);
}
