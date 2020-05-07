// tsig.c ... functions on Tuple Signatures (tsig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"

Bits codewords(char *attr_value, int m, int k);

// generate codewords

Bits codewords(char *attr_value, int m, int k) {
	int nbits = 0;
	Bits cword = newBits(m);
	srandom(hash_any(attr_value, strlen(attr_value)));
	while (nbits < k) {
		int i = random() % m;
		if (!bitIsSet(cword, i)) {
			setBit(cword, i);
			nbits++;
		}
	}
	return cword;
}

// make a tuple signature

Bits makeTupleSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	int tsigbit = tsigBits(r);
	int codebit = codeBits(r);
	Bits desc = newBits(tsigbit);
	int nattr = nAttrs(r);
	char **attr_val = tupleVals(r, t);
	for (int i = 0; i < nattr; i++) {
		if (strcmp(attr_val[i], "?") != 0) {
			Bits cw = codewords(attr_val[i], tsigbit, codebit);
			orBits(desc, cw);
		}
	}
	return desc;
}

// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	//TODO
	Reln r = q->rel;
	Bits Querysig = makeTupleSig(r, q->qstring);
	Bits tsig = newBits(tsigBits(r));
	for (int pid = 0; pid < nTsigPages(r); pid++) {
		Page p = getPage(tsigFile(r), pid);
		for (int tid = 0; tid < pageNitems(p); tid++) {
			getBits(p, tid, tsig);
			if (isSubset(Querysig, tsig)) {
				// compute the corresponding pageid of this tuple
				PageID pageid = q->nsigs / maxTupsPP(r);
				setBit(q->pages, pageid);
			}
			// every signature 
			q->nsigs++;
		}
		q->nsigpages++;
	}

	// The printf below is primarily for debugging
	// Remove it before submitting this function
	// printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
