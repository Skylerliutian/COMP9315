// insert into a page with fixed-size records
// and compacted free space

// Assume the following definitions

typedef ... Byte;    // one-byte value
typedef ... Count;   // two-byte value

#define PAGESIZE ...

typedef struct {
    Count   nrecs;     // # stored records
    Count   maxrecs;   // max stored records (# slots)
    Record  data[1];   // start of tuples
} Page;

// Initialise Page data

void initPage(Page *p, Count size)
{
    p->nrecs = 0;
    p->maxrecs = (PAGESIZE - sizeof(Page) + 1)/sizeof(Record);
}

// Add record into Page; return index of new record

int insert(Page *p, Record *r)
{
    if (p->nrecs == p->maxrecs) FAIL no room;
    Byte *location = &(p->data[p->nrecs]);
    memcpy(location, r, sizeof(Record));
    int index = p->nrecs;
    p->nrecs++;
    return index;
}

// Delete record from Page;

void delete(Page *p, Index i)
{
    // compacted free space => can't leave gaps
    if (i < p->nrecs-1) {
        // copy top record into gap
        memcpy(&(p->rec[i]), &(p->rec[p->nrecs-1]), RecSize);
        // PROBLEM!
        // RecordId of previous top Record has changed!
    }
    p->nrecs--;
}
