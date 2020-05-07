// bufpool.c ... buffer pool implementation

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bufpool.h"

#define MAXID 4

// for Cycle strategy, we simply re-use the nused counter
// since we don't actually need to maintain a usedList
#define currSlot nused

// one buffer
struct buffer {
	char  id[MAXID];
	int   pin;
	int   dirty;
	char  *data;
};

// collection of buffers + stats
struct bufPool {
	int   nbufs;         // how many buffers
	char  strategy;      // LRU, MRU, Cycle
	int   nrequests;     // stats counters
	int   nreleases;
	int   nhits;
	int   nreads;
	int   nwrites;
	int   *freeList;     // list of completely unused bufs
	int   nfree;
	int   *usedList;     // implements replacement strategy
	int   nused;
	struct buffer *bufs;
};

static clock = 0;


// Helper Functions (private)


// pageInBuf(pool,slot)
// - return the name of the page current stored in specified slot

static
char *pageInBuf(BufPool pool, int slot)
{
	char *pname;
	pname = pool->bufs[slot].id;
	if (pname[0] == '\0')
		return "_";
	else
		return pname;
}

// pageInPool(BufPool pool, char rel, int page)
// - check whether page from rel is already in the pool
// - returns the slot containing this page, else returns -1

static
int pageInPool(BufPool pool, char rel, int page)
{
	int i;  char id[MAXID];
	sprintf(id,"%c%d",rel,page);
	for (i = 0; i < pool->nbufs; i++) {
		if (strcmp(id,pool->bufs[i].id) == 0) {
			return i;
		}
	}
	return -1;
}

// removeFirstFree(pool)
// - use the first slot on the free list
// - the slot is removed from the free list
//   by moving all later elements down

static
int removeFirstFree(BufPool pool)
{
	int v, i;
	assert(pool->nfree > 0);
	v = pool->freeList[0];
	for (i = 0; i < pool->nfree-1; i++)
		pool->freeList[i] = pool->freeList[i+1];
	pool->nfree--;
	return v;
}

// removeFromUsedList(pool,slot)
// - search for a slot in the usedList and remove it
// - depends on how usedList managed, so is strategy-dependent

static
void removeFromUsedList(BufPool pool, int slot)
{
	int i, j;
	switch (pool->strategy) {
	case 'L':
	case 'M':
		// find where this slot appears in the usedList
		j = -1;
		for (i = 0; i < pool->nused; i++) {
			if (pool->usedList[i] == slot) {
				j = i;
				break;
			}
		}
		// if it's there, remove it
		if (j >= 0) {
			for (i = j; i < pool->nused-1; i++)
				pool->usedList[i] = pool->usedList[i+1];
			pool->nused--;
		}
		break;
	case 'C':
		// since no used list, nothing to do
		break;
	}
}

// getNextSlot(pool)
// - finds the "best" unused buffer pool slot
// - "best" is determined by the replacement strategy
// - if the replaced page is dirty, write it out
// - initialise the chosen slot to hold the new page
// - if there are no available slots, return -1

static
int grabNextSlot(BufPool pool)
{
	int slot;
	switch (pool->strategy) {
	case 'L':
		// get least recently used slot from used list
		if (pool->nused == 0) return -1;
		slot = pool->usedList[0];
		int i;
		for (i = 0; i < pool->nused-1; i++)
			pool->usedList[i] = pool->usedList[i+1];
		pool->nused--;
		break;
	case 'M':
		// get most recently used slot from used list
		if (pool->nused == 0) return -1;
		slot = pool->usedList[pool->nused-1];
		pool->nused--;
		break;
	case 'C':
		// get next available according to cycle counter
		slot = -1;
		for (i = 0; i < pool->nbufs; i++) {
			int s;
			s = (pool->currSlot + i) % pool->nbufs;
			if (pool->bufs[s].pin == 0) {
				slot = s;
				break;
			}
		}
		if (slot >= 0)
			pool->currSlot = (slot+1) % pool->nbufs;
		break;
	}
	if (slot >= 0 && pool->bufs[slot].dirty) {
		pool->nwrites++;
		pool->bufs[slot].dirty = 0;
	}
	return slot;
}


// makeAvailable(pool,slot)
// - add the specified slot to the used list
// - where to add depends on strategy

static
void makeAvailable(BufPool pool, int slot)
{
	switch (pool->strategy) {
	case 'L':
	case 'M':
		// slot become most recently used
		pool->usedList[pool->nused] = slot;
		pool->nused++;
		break;
	case 'C':
		// slot becomes available
		// nothing to do here ...
		break;
	}
}


// Interface Functions


// initBufPool(nbufs,strategy)
// - initialise a buffer pool with nbufs
// - buffer pool uses supplied replacement strategy

BufPool initBufPool(int nbufs, char strategy)
{
	BufPool newPool;
	struct buffer *bufs;

	newPool = malloc(sizeof(struct bufPool));
	assert(newPool != NULL);
	newPool->nbufs = nbufs;
	newPool->strategy = strategy;
	newPool->nrequests = 0;
	newPool->nreleases = 0;
	newPool->nhits = 0;
	newPool->nreads = 0;
	newPool->nwrites = 0;
	newPool->nfree = nbufs;
	newPool->nused = 0;
	newPool->freeList = malloc(nbufs * sizeof(int));
	assert(newPool->freeList != NULL);
	newPool->usedList = malloc(nbufs * sizeof(int));
	assert(newPool->usedList != NULL);
	newPool->bufs = malloc(nbufs * sizeof(struct buffer));
	assert(newPool->bufs != NULL);

	int i;
	for (i = 0; i < nbufs; i++) {
		newPool->bufs[i].id[0] = '\0';
		newPool->bufs[i].pin = 0;
		newPool->bufs[i].dirty = 0;
		newPool->freeList[i] = i;
		newPool->usedList[i] = -1;
	}
	return newPool;
}

int request_page(BufPool pool, char rel, int page)
{
	int slot;
	printf("Request %c%d\n", rel, page);
	pool->nrequests++;
	slot = pageInPool(pool,rel,page);
	if (slot >= 0)
		pool->nhits++;
	else { // page is not already in pool
		if (pool->nfree > 0) {
			slot = removeFirstFree(pool);
		}
		else {
			slot = grabNextSlot(pool);
		}
		if (slot < 0) {
			fprintf(stderr, "Failed to find slot for %c%d\n",rel,page);
			exit(1);
		}
		pool->nreads++;
		sprintf(pool->bufs[slot].id,"%c%d",rel,page);
		pool->bufs[slot].pin = 0;
		pool->bufs[slot].dirty = 0;
	}
	// have a slot
	pool->bufs[slot].pin++;
	removeFromUsedList(pool,slot);
	showPoolState(pool);  // for debugging
	return slot;
}

void release_page(BufPool pool, char rel, int page)
{
	printf("Release %c%d\n", rel, page);
	pool->nreleases++;

	int i;
	i = pageInPool(pool,rel,page);
	assert(i >= 0);
	// last user of page is about to release
	if (pool->bufs[i].pin == 1) {
		makeAvailable(pool, i);
	}
	pool->bufs[i].pin--;
	//showPoolState(pool);  // for debugging
}

// showPoolUsage(pool)
// - prints statistics counters for buffer pool

void showPoolUsage(BufPool pool)
{
	assert(pool != NULL);
	printf("#requests: %d\n",pool->nrequests);
	printf("#releases: %d\n",pool->nreleases);
	printf("#hits    : %d\n",pool->nhits);
	printf("#reads   : %d\n",pool->nreads);
	printf("#writes  : %d\n",pool->nwrites);
}

// showPoolState(pool)
// - display printable representation of buffer pool on stdout

void showPoolState(BufPool pool)
{
#if 1
	int i; char *p;
	for (i = 0; i < pool->nbufs; i++) {
		p = pool->bufs[i].id;
		if (p[0] == '\0') p = "-";
		printf("%5s", p);
	}
	printf("\n");
#else
	int i, j; char *p; struct buffer b;

	printf("%4s %6s %6s %6s\n","Slot","Page","Pin","Dirty");
	for (i = 0; i < pool->nbufs; i++) {
		b = pool->bufs[i];
		p = pageInBuf(pool,i);
		printf("[%02d] %6s %6d %6d\n", i, p, b.pin, b.dirty);
	}
	printf("FreeList:");
	for (i = 0; i < pool->nfree; i++) {
		j = pool->freeList[i];
		printf(" [%02d]%s", j, pageInBuf(pool,j));
	}
	printf("\n");
	printf("UsedList:");
	for (i = 0; i < pool->nused; i++) {
		j = pool->usedList[i];
		printf(" [%02d]%s", j, pageInBuf(pool,j));
	}
	printf("\n");
#endif
}
