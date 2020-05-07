// chvec.h ... interface to functions on ChoiceVectors
// A ChVec is an array of MAXCHVEC ChVecItems
// Each ChVecItem is a pair (attr#,bit#)

#ifndef CHVEC_H
#define CHVEC_H 1

#include "defs.h"

#define MAXCHVEC 32

typedef struct _ChVecItem { Byte att; Byte bit; } ChVecItem;

typedef ChVecItem ChVec[MAXCHVEC];

int parseChVec(int nattrs, char *str, ChVec cv);
void printChVec(ChVec cv);

#endif
