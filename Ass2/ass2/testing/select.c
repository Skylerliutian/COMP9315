// select.c ... run queries
// part of SIMC signature files
// Ask a query on a named relation
// Usage:  ./select  [-v]  RelName  v1,v2,v3,v4,...  Sigs
// where any of the vi's can be "?" (unknown)

#include "defs.h"
#include "query.h"
#include "tuple.h"
#include "reln.h"

#define USAGE "./select  [-v]  RelName  v1,v2,v3,v4,...  [t|p|b]"

// Main ... process args, run query

int main(int argc, char **argv)
{
	Reln r;       // open relation info
	Query q;      // query iteration information
	int verbose;  // show extra info on query progress
	char *rname;  // name of table/file
	char *qstr;   // query string
	char  type = '?';   // type of signatures to use
	char err[MAXERRMSG];  // buffer for error messages

	// process command-line args

	if (argc < 3) fatal(USAGE, "");
	if (strcmp(argv[1], "-v") == 0) {
		verbose = 1;  rname = argv[2];  qstr = argv[3];
        if (argc > 4) type = argv[4][0];
	}
	else {
		verbose = 0;  rname = argv[1];  qstr = argv[2];
		if (argc > 3) type = argv[3][0];
	}

	if (verbose) { /* keeps compiler quiet */ }

	// initialise relation and scan descriptors

	if ((r = openRelation(rname)) == NULL) {
		sprintf(err, "Can't open relation: %s",rname);
		fatal("", err);
	}
	if ((q = startQuery(r, qstr, type)) == NULL) {	
		sprintf(err, "Invalid query: %s",qstr);
		fatal("",err);
	}

	// scan selected pages to find matching tuples
	scanAndDisplayMatchingTuples(q);

	printf("Query Stats:\n"); queryStats(q);

	// clean up
	closeQuery(q);
	closeRelation(r);

	return 0;
}

