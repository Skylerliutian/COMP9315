// Assume the following definitions

typedef ... Byte;    // one-byte value
typedef ... Count;   // two-byte value
typedef ... Offset;  // two-byte value

#define PAGESIZE ...

typedef struct {
    Offset  freeTop;
    Count   nrecs;
    Offset  rec[1];  // start of directory
                     // data space 
} Page;

// Initialise Page data

void initPage(Page *p)
{
    p->nrecs = 0;
    p->freeTop = PAGESIZE-1;
    Byte *data;
    data = &(p->rec[0]);
    int nBytes = PAGESIZE - 2*sizeof(Byte2);
    for (int i = 0; i < nBytes; i++) data[i] = 0;
}

// Add record into Page; return record index

int insert(Page *p, Record *r)
{
    // TODO
}
