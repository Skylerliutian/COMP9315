// bnl.c ... simulate buffered nested loop join

#include <stdio.h>
#include <stdlib.h>
#include "bufpool.h"

void exitUsage(char *);

// main ... process args and run simulation

int main(int argc, char **argv)
{
    int  outer, inner, nbufs;

    // collect argv values
    if (argc < 4) { exitUsage(argv[0]); }
    sscanf(argv[1], "%d", &outer);
    sscanf(argv[2], "%d", &inner);
    sscanf(argv[3], "%d", &nbufs);

    // remind us of the parameters
    printf("Running: ./bnl %d %d %d\n", outer, inner, nbufs);

    // initalise buffer pool
    BufPool pool = initBufPool(nbufs);

    // simulate the nested loop join
    // select * from R join S ...
    int i, j;
    for (i = 0; i < outer; i++) {
        request_page(pool,'R',i);
        for (j = 0; j < inner; j++) {
            request_page(pool,'S',j);
            // process Xi and Yj
            release_page(pool,'S',j);
        }
        release_page(pool,'R',i);
    }

    // show final state of buffer pool
    showPoolState(pool);
    printf("\n");
    // show buffer pool usage
    showPoolUsage(pool);
}

void exitUsage(char *prog)
{
    fprintf(stderr, "Usage: %s OuterPages InnerPages Slots\n", prog);
    exit(1);
}
