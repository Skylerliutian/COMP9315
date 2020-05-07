// bsig.c ... functions on Tuple Signatures (bsig's)
// part of SIMC signature files
// Written by John Shepherd, March 2020

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "psig.h"

void findPagesUsingBitSlices(Query q)
{
	assert(q != NULL);
	//TODO
	Reln r = q->rel;
	PageID ppid = -1;
	Page p;
	Bits Querysig = makePageSig(r, q->qstring);
	setAllBits(q->pages);
	for (int i = 0; i < psigBits(r); i++) {
		Bits bsig = newBits(bsigBits(r));
		if ( bitIsSet(Querysig, i) )
		{
			// bid = i / maxBsigsPP(q->rel);
			//printf("bsigPid:%d\n", bsigPid);
			
			if ( i / maxBsigsPP(r) != ppid){
				q->nsigpages++;
				ppid = i / maxBsigsPP(r);
				p = getPage(bsigFile(r), ppid);
				//printf("bsigPid:%d\n", bsigPid);
			}
			Count off_t = i % maxBsigsPP(r);
		
			getBits(p, off_t, bsig);
			
			// printf("%d, %d \n", nPsigs(r), nPages(r));
			for (int j = 0; j < nPsigs(r); j++)
			{
				if(bitIsSet(bsig, j) == FALSE){
					unsetBit(q->pages, j);
				}
			}
			q->nsigs++;
		}
	}
	
}

