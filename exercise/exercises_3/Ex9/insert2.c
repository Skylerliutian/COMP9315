// Assume the following definitions

typedef ... Byte;    // one-byte value
typedef ... Count;   // two-byte value
typedef ... Offset;  // two-byte value

#define PAGESIZE ...

typedef struct {
    Offset  freeTop;
    Count   nrecs;
    Byte    rec[1];  // beginning of directory
} Page;

typedef struct {
    Offset   size;
    Offset   next;
} FreeChunk;

// Initialise Page data

void initPage(Page *p)
{
    p->nrecs = 0;
    p->freeList = PAGESIZE-1;
    Byte *data;
    data = &(p->rec);
    int nBytes = PAGESIZE - 2*sizeof(Offset);;
    for (int i = 0; i < nBytes; i++) data[i] = 0;
    FreeChunk *free = p + PAGESIZE-1 - sizeof(FreeChunk);
    free->size = PAGESIZE-2*sizeof(Offset);
    free->next = 0; // not valid page offset
}

// Add record into Page; return record index

int insert(Page *p, Record *r)
{
    // TODO
    // find free space for tuple
    // copy record data there
    // update directory, etc.
}
