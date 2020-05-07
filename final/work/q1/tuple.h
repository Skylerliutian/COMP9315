// tuple.h ... interface to functions on Tuples
// A Tuple is just a '\0'-terminated C string
// Consists of "val_1,val_2,val_3,...,val_n"

#ifndef TUPLE_H
#define TUPLE_H 1

typedef char *Tuple;

#include "defs.h"
#include "chvec.h"
#include "bits.h"

int tupLength(Tuple t);
Bits tupleHash(ChVec cv, Count nattrs, Tuple t);
void tupleVals(Tuple t, char **vals);
void freeVals(char **vals, int nattrs);
void tupleString(Tuple t, char *buf);

#endif
