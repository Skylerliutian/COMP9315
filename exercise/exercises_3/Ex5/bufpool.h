// bufpool.h ... buffer pool interface

typedef struct bufPool *BufPool;

BufPool initBufPool(int, char);
int     request_page(BufPool, char, int);
void    release_page(BufPool, char, int);
void    showPoolUsage(BufPool);
void    showPoolState(BufPool);
