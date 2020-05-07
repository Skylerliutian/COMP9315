// chvec.c ... functions on ChoiceVectors
// A ChVec is an array of MAXCHVEC ChVecItems
// Each ChVecItem is a pair (attr#,bit#)

#include "defs.h"
#include "chvec.h"

// convert a a,b:a,b:a,b:...:a,b" representation
//  of a choice vector into a ChVec
// if string doesn't specify all 32 bits, then
//  cycle through attributes until reach 32 bits

int parseChVec(int nattrs, char *str, ChVec cv)
{
    Count i = 0;
    char *c = str, *c0 = str;
    while (*c != '\0') {
        while (*c != ':' && *c != '\0') c++;
        Count a, b, n;
        if (*c == '\0') {
            n = sscanf(c0, "%d,%d", &a, &b);
            // is the (attr,bit) pair valid?
            // neither a nor b can be < 0 because they're unsigned
            if (n != 2 || a >= nattrs || b >= 32) {
                printf("Invalid choice vector element: (att:%d,bit:%d)\n",a,b);
                return 0;
            }
        }
        else {
            *c = '\0';
            n = sscanf(c0, "%d,%d", &a, &b);
            if (n != 2 || a >= nattrs || b >= 32) {
                printf("Invalid choice vector element: (att:%d,bit:%d)\n",a,b);
                return 0;
            }
            *c = ':'; c++; c0 = c;
        }
        cv[i].att = a; cv[i].bit = b;
        //printf("cv[%d] is (%d,%d)\n", i, cv[i].att, cv[i].bit);
        i++;
    }
    // get enough bits for a 32-bit choice vector
    // take new bits from top end of each hash,
    //   so as to hopefully not conflict 
    Count x;  Count next[MAXCHVEC];
    for (x = 0; x < MAXCHVEC; x++) next[x] = 31;
    x = 0;
    while (i < MAXCHVEC) {
        cv[i].att = x; cv[i].bit = next[x];
        //printf("cv[%d] is (%d,%d)\n", i, cv[i].att, cv[i].bit);
        next[x]--;
        i++; x = (x+1) % nattrs;
    }
    return 1;
}

// print a choice vector (for debugging)

void printChVec(ChVec cv)
{
    int i;
    for (i = 0; i < MAXCHVEC; i++) {
        printf("%d,%d",cv[i].att, cv[i].bit);
        if (i < MAXCHVEC-1) putchar(':');
    }
    printf("\n");
}
