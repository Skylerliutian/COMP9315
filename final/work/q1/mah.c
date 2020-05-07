// mah.c ... generate multi-attribute hash values

#include <stdio.h>
#include <stdlib.h>
#include "bits.h"
#include "chvec.h"
#include "tuple.h"

void exitUsage(char *);

// main ... process args and run simulation

int main(int argc, char **argv)
{
    char *cv, *tup;
    ChVec chvec;

    // collect argv values
    if (argc < 3) { exitUsage(argv[0]); }
    cv = argv[1];
    tup = argv[2];

    // count number of attributes
    int nattrs = 1;
    for (char *c = tup; *c != '\0'; c++) {
        if (*c == ',') nattrs++;
    }

    // build choice vector
    if (!parseChVec(nattrs, cv, chvec)) {
        fprintf(stderr, "Invalid choice vector: %s\n", cv);
        exit(1);
    }

    // form MAH value
    Bits hash = tupleHash(chvec, nattrs, tup);

    // display result
    char buf[MAXBITS+1];
    bitsString(hash,buf);
    printf("hash(%s) = %s\n", tup, buf);

    exit(0);
}

void exitUsage(char *prog)
{
    fprintf(stderr, "Usage: %s ChoiceVector Tuple\n", prog);
    exit(1);
}
