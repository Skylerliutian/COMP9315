// create.c ... create an empty Relation
// part of superimposed codeword signature files
// Ask a query on a named file
// Usage:  ./create  RelName  #attrs  1/pF
// where #attrs = #attributes in each tuple
//		tupSize = #bytes in each tuple
//		  pF = inverse of false match prob

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "util.h"
#include "reln.h"

#define USAGE "./create  RelName  #tuples  #attrs  1/pF"


// Main ... process args, run query

int main(int argc, char **argv)
{
	char err[200];  // buffer for error messages

	// Process command-line args

	if (argc < 5) fatal(USAGE, "");

	// how many tuples expected
	int ntuples = atoi(argv[2]);
	if (ntuples < 10) {
		sprintf(err, "Invalid #tuples: %d (must be >= 10)", ntuples);
		fatal("", err);
	}

	// how many attributes in each tuple
	int nattrs = atoi(argv[3]);
	if (nattrs < 2 || nattrs > 9) {
		sprintf(err, "Invalid #attrs: %d (must be 1 < # < 10)", nattrs);
		fatal("", err);
	}

	// false match probability
	float pF = 1.0 / atoi(argv[4]);
	if (pF > 0.01) {
		sprintf(err, "Invalid pF: %f (must be > 100)", pF);
		fatal("", err);
	}

	// compute parameters, based on argv
	// all tuples look like:
	// 7-digits,20-alphas,6-alphanum,... up to nattrs
	Count tsize = 28 + 7*(nattrs-2);
	Count capacity = (PAGESIZE-sizeof(Count))/tsize;
	double log2 = 1.0/log(2.0);
	double logF = log(1.0/(double)pF);
	Count tk  = (int)(log2 * logF);
	Count tm  = (int)(log2*log2 * nattrs * logF);
	Count pm  = (int)(log2*log2 * nattrs*capacity * logF);
	// HACK: we need a value for bm, but bm = #pages
	// HACK: we have no pages yet, so set bit-slices to 4K bits
	// HACK:   assuming that we never need more than 4K data pages
	Count bm  = ntuples / capacity;
	if (ntuples%capacity > 0) bm++;

	// create relation, unless it exists already
	if (existsRelation(argv[1])) {
		sprintf(err, "Relation %s already exists", argv[1]);
		fatal("", err);
	}
	if (newRelation(argv[1], nattrs, pF, tk, tm, pm, bm) != OK) {
		sprintf(err, "Problems while creating relation %s", argv[1]);
		fatal("", err);
	}

	return OK;
}
