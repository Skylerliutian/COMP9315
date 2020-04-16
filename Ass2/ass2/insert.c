// insert.c ... add tuples to a relation
// part of SIMC signature files
// Reads tuples from stdin and inserts into Reln
// Usage:  ./insert  [-v]  RelName
// Written by John Shepherd, March 2020

#include "defs.h"
#include "reln.h"
#include "tuple.h"

#define USAGE "./insert  [-v]  RelName"

// Main ... process args, read/insert tuples

int main(int argc, char **argv)
{
	Reln r;  // handle on the open relation
	char err[MAXERRMSG];  // buffer for error messages
	int verbose;  // show extra info on query progress
	char *rname;  // name of table/file

	// process command-line args

	if (argc < 2) fatal(USAGE, "");
	if (strcmp(argv[1], "-v") == 0)
		{ verbose = 1; rname = argv[2]; }
	else
		{ verbose = 0; rname = argv[1]; }


	// set up relation for writing

	if ((r = openRelation(rname)) == NULL) {
		sprintf(err, "Can't open relation: %s",rname);
		fatal("", err);
	}

	// read stdin and insert tuples

	Tuple t;  PageID pid;
	while ((t = readTuple(r,stdin)) != NULL) {
		//printf("Inserting: "); showTuple(r,t);
		pid = addToRelation(r,t);
		if (pid == NO_PAGE) {
			sprintf(err, "Insert of %s failed\n", t);
			fatal("",err);
		}
		if (verbose) printf("%s -> %d\n",t,pid);
		free(t);
	}

	// clean up

	closeRelation(r);

	return 0;
}

