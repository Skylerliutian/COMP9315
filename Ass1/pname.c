/*
 * src/tutorial/pname.c
 * 
 * COMP9315 Database Systems Impmentation
 * Tian Liu
 * Assignment 1 Adding a PersonName Data Type to PostgreSQL
 * 
 * 
 *
 ******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */
#include <ctype.h>
#include <string.h>
PG_MODULE_MAGIC;

typedef struct Pname
{
	char *family;
	char *given;
} PersonName;


/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_in);
/**
 * Check the input is valid or not
 * return True/False
 */
bool validName(char *str){
	int divide = 0;
	int i;
	// find the comma and total length
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == ',') {
			divide = i;
		}
	}
	int total_len = i;
	// check input contains comma 
	if (divide == 0) {return False;}
	// space before comma and in the begining are not allowed
	if (str[divide - 1] == ' ' || str[0] == ' ') {return Flase;}
	
	// create family and given two lists
	char family[divide + 1];
	char given[total_len - divide + 1];
	for (int i = 0; i < divide; i++) {
		family[i] = str[i];
	}
	family[divide] = '\0';
	int j = 0;
	for (int i = divide + 1; i <= total_len; i++) {
		given[j] = str[i];
		j++;
	}
	// split family by space
	char *delim = " ";
    char *p;
	p = strtok(family, delim);
	// names must at least 2 letters
	if (strlen(p) < 2) {return False;}
	// the first letter must be uppercase
	if (!isupper(*p)) {return False;}
	p++;
	while (*p != '\0') {
		if (isalpha(*p)||*p == '-'|| *p == '\'') {
			p++;
		}
		else {
			return False;
		}
	}
	while ((p = strtok(NULL, delim))) {
		if (strlen(p) < 2) {return False;}
		if (!isupper(*p)) {return False;}
		p++;
		while (*p != '\0') {
			if (isalpha(*p)||*p == '-'|| *p == '\'') {
				p++;
			}
			else {
				return False;
			}
		}
	}
	// split the given part
	char *g;
	g = strtok(given, delim);
	// names must at least 2 letters
	if (strlen(g) < 2) {return False;}
	// the first letter must be uppercase
	if (!isupper(*g)) {return False;}
	g++;
	while (*g != '\0') {
		if (isalpha(*g)||*gg == '-'|| *g == '\'') {
			g++;
		}
		else {
			return False;
		}
	}
	while ((g = strtok(NULL, delim))) {
		if (strlen(p) < 2) {return False;}
		if (!isupper(*g)) {return False;}
		g++;
		while (*g != '\0') {
			if (isalpha(*g)||*g == '-'|| *g == '\'') {
				g++;
			}
			else {
				return False;
			}
		}
	}
	return True;
}

Datum
pname_in(PG_FUNCTION_ARGS)
{
	char *str = PG_GETARG_CSTRING(0);
	char *token;
	PersonName *result;
	// to check whether input is valid
	if (!validName(str))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for PersonName %s", str)));
	result = (PersonName *) palloc(sizeof(PersonName));
	token = strtok(str, ",");
	strcpy(result->family, token);
	token = strtok(NULL, ",");
	strcpy(result->given, token);
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *result;
	result = (char *) palloc(sizeof(PersonName));
	// sprintf(result, "%s,%s", result->family, result->given);
	snprintf(result, sizeof(PersonName), "%s,%s", result->family, result->given);
	PG_RETURN_CSTRING(result);
}

/*****************************************************************************
 * Binary Input/Output functions
 *
 * These are optional.
 *****************************************************************************/

PG_FUNCTION_INFO_V1(pname_recv);

Datum
pname_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	PersonName    *result;

	result = (PersonName *) palloc(sizeof(PersonName));
	strcpy(result->family, pq_getmsgstring(buf));
	strcpy(result->given, pq_getmsgstring(buf));
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(pname_send);

Datum
pname_send(PG_FUNCTION_ARGS)
{
	PersonName    *pname = (PersonName *) PG_GETARG_POINTER(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, pname->family);
	pq_sendfloat8(&buf, pname->given);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

// /*****************************************************************************
//  * New Operators
//  *
//  * A practical Complex datatype would provide much more than this, of course.
//  *****************************************************************************/

// PG_FUNCTION_INFO_V1(complex_add);

// Datum
// complex_add(PG_FUNCTION_ARGS)
// {
// 	Complex    *a = (Complex *) PG_GETARG_POINTER(0);
// 	Complex    *b = (Complex *) PG_GETARG_POINTER(1);
// 	Complex    *result;

// 	result = (Complex *) palloc(sizeof(Complex));
// 	result->x = a->x + b->x;
// 	result->y = a->y + b->y;
// 	PG_RETURN_POINTER(result);
// }


/*****************************************************************************
 * Operator class for defining B-tree index
 *
 * It's essential that the comparison operators and support function for a
 * B-tree index opclass always agree on the relative ordering of any two
 * data values.  Experience has shown that it's depressingly easy to write
 * unintentionally inconsistent functions.  One way to reduce the odds of
 * making a mistake is to make all the functions simple wrappers around
 * an internal three-way-comparison function, as we do here.
 *****************************************************************************/


static int
pname_cmp_internal(PersonName * a, PersonName * b)
{	
	if (*(a->given) == ' ') {
		a->given++;
	}
	if (*(b->given) == ' ') {
		b->given++;
	}
	if (strcmp(a->family, b->family) > 0) {
		return 1;	
	}
	else if (strcmp(a->family, b->family) < 0) {
		return -1;
	}
	else {
		if (strcmp(a->given, b->given) > 0) {
			return 1;
		}
		else if (strcmp(a->given, b->given) < 0) {
			return -1;
		}
		else {
			return 0;
		}
	}
}

PG_FUNCTION_INFO_V1(pname_lt);

Datum
pname_lt(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *) PG_GETARG_POINTER(1);
	PG_RETURN_BOOL(pname_cmp_internal(a, b) < 0);
}

PG_FUNCTION_INFO_V1(pname_le);

Datum
pname_le(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *) PG_GETARG_POINTER(1);
	PG_RETURN_BOOL(pname_cmp_internal(a, b) <= 0);
}

PG_FUNCTION_INFO_V1(pname_eq);

Datum
pname_eq(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *) PG_GETARG_POINTER(1);
	PG_RETURN_BOOL(pname_cmp_internal(a, b) == 0);
}

PG_FUNCTION_INFO_V1(pname_neq);

Datum
pname_neq(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *) PG_GETARG_POINTER(1);
	PG_RETURN_BOOL(pname_cmp_internal(a, b) != 0);
}

PG_FUNCTION_INFO_V1(pname_ge);

Datum
pname_ge(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *) PG_GETARG_POINTER(1);
	PG_RETURN_BOOL(pname_cmp_internal(a, b) >= 0);
}

PG_FUNCTION_INFO_V1(pname_gt);

Datum
pname_gt(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *) PG_GETARG_POINTER(1);
	PG_RETURN_BOOL(pname_cmp_internal(a, b) > 0);
}

PG_FUNCTION_INFO_V1(pname_family);

Datum
pname_family(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char result[strlen(pname->family) + 1];
	sttcpy(result, pname->family);
	PG_RETURN_CSTRING(result);

}

PG_FUNCTION_INFO_V1(pname_given);

Datum
pname_given(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char result[strlen(pname->given) + 1];
	sttcpy(result, pname->given);
	PG_RETURN_CSTRING(result);
}


// PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
// 	char family[strlen(pname->family) + 1];
// 	strcpy(family, pname->family); 
// 	char *result;
// 	char *delim = " ";
// 	result = strtok(family, delim);
// 	PG_RETURN_CSTRING(result);