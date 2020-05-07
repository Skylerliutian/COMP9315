// psig.c ... functions on page signatures (psig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"

Bits psig_codewords(char *attr_value, int m, int k);

// generate codewords

Bits psig_codewords(char *attr_value, int m, int k) {
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

Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	int psigbit = psigBits(r);
	int codebit = codeBits(r);
	Bits desc = newBits(psigbit);
	int nattr = nAttrs(r);
	char **attr_val = tupleVals(r, t);
	for (int i = 0; i < nattr; i++) {
		if (strcmp(attr_val[i], "?") != 0) {
			Bits cw = psig_codewords(attr_val[i], psigbit, codebit);
			orBits(desc, cw);
		}
	}
	return desc;
}

void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	//TODO
	Reln r = q->rel;
	Bits Querysig = makePageSig(r, q->qstring);
	Bits psig = newBits(psigBits(r));
	for (int pid = 0; pid < nPsigPages(r); pid++) {
		Page p = getPage(psigFile(r), pid);
		for (int num = 0; num < pageNitems(p); num++) {
			getBits(p, num, psig);
			if (isSubset(Querysig, psig)) {
				PageID PID = q->nsigs;
				setBit(q->pages, PID);
			}
			q->nsigs++;
		}
		q->nsigpages++;
	}
}

