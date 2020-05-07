// insert into a page with fixed-size records
// and distributed free space

// Assume the following definitions

typedef ... Byte;    // one-byte value
typedef ... Count;   // two-byte value

#define PAGESIZE ...

typedef struct {
    Bit     present[MAXRECS];  // MAXRECS??
    Record  data[1];   // start of tuples
} Page;

// Initialise Page data

void initPage(Page *p, Count size)
{
    for (int i = 0; i < MAXRECS; i++)
        present[i] = 0;
}

// Add record into Page; return record index

int insert(Page *p, Record *r)
{
    // find a free slot
    int slot = -1;
    for (int i = 0; i < MAXRECS; i++)
        if (present[i] == 0) {
            slot = i;
            break;
        }
    }
    if (slot < 0) FAIL no room;
    Byte *location = &(p->data[slot]);
    memcpy(location, r, sizeof(Record));
    return slot;
}

// Delete record from Page;

void delete(Page *p, Index i)
{
    p->present[i] = 0;
}
