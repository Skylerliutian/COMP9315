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
#include "access/hash.h"
PG_MODULE_MAGIC;

typedef struct Pname
{
	int length;
	// just define an array which is variable
	char name[1];
} PersonName;


/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/
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

PG_FUNCTION_INFO_V1(pname_in);

Datum
pname_in(PG_FUNCTION_ARGS)
{
	char *str = PG_GETARG_CSTRING(0);
	PersonName *result;
	int length = strlen(str) + 1;
	// to check whether input is valid
	if (!validName(str))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"pname", str)));
	result = (PersonName *) palloc(VARHDRSZ + length);
	SET_VARSIZE = (result, VARHDRSZ + length);
	snprintf(result->name, length , "%s", str);
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *result;
	result = psprintf("%s", pname->name);
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
	char *personname = pq_getmsgstring(buf);
	int length = strlen(buf) + 1;
	result = (PersonName *) palloc(VARHDRSZ + length);
	SET_VARSIZE = (result, VARHDRSZ + length);
	snprintf(result->name, length , "%s", personname);
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(pname_send);

Datum
pname_send(PG_FUNCTION_ARGS)
{
	PersonName    *pname = (PersonName *) PG_GETARG_POINTER(0);
	StringInfoData buf;
	pq_begintypsend(&buf);
	pq_sendstring(&buf, pname->name);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

/*****************************************************************************
 * Operators
 *
 * A practical PersonName datatype would provide much more than this, of course.
 *****************************************************************************/

static int
pname_cmp_internal(PersonName * a, PersonName * b)
{	
	int a_divide, b_divide;
	int result;
	for (a_divide = 0; a_divide < strlen(a->name); a_divide++) {
		if (a->name[a_divide] == ',') {
			break;
		}
	}
	for (a_divide = 0; a_divb_divideide < strlen(b->name); b_divide++) {
		if (b->name[b_divide] == ',') {
			break;
		}
	}
	char *a_given = &a->name[a_divide + 1];
	char *b_given = &b->name[b_divide + 1];
	a->name[a_divide] = '\0';
	b->name[b_divide] = '\0';
	result = strcmp(a->name, b->name);
	a->name[a_divide] = ',';
	b->name[b_divide] = ',';
	if (result == 0) {
		if (*a_given == ' ') {
			a_given++;
		}
		if (*b_given == ' ') {
			b_given++;
		}
		result = strcmp(a_given, b_given);
	}
	return result;
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

PG_FUNCTION_INFO_V1(family);

Datum
family(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *result;
	int divide;
	for (divide = 0; divide < strlen(pname->name); divide++) {
		if (pname->name[a_divide] == ',') {
			break;
		}
	}
	pname->name[divide] = '\0';
	result = psprintf("%s", pname->name);
	pname->name[divide] = ',';
	PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(given);

Datum
given(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *result;
	result = strrchr(pname->name, ',');
	result++;
	if (*result == ' ') {
		result++;
	}
	result = psprintf("%s", pname->name);
	PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(pname_hash);

Datum
pname_hash(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	
	int h_code;
	h_code = DatumGetUInt32(hash_any((unsigned char *) pname->name, strlen(pname->name)));
	
	PG_RETURN_INT32(h_code);
}
