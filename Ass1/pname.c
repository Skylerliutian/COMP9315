/*
 * src/tutorial/pname.c
 * 
 * COMP9315 Database Systems Impmentation
 * z5230310, Tian Liu
 * z5151539, Tianyu Deng
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
#include "access/hash.h"
#include <string.h>
#include <stdbool.h>
#include <regex.h>

PG_MODULE_MAGIC;

typedef struct Pname
{
	int length;
	// just define an array which is variable
	char name[1];
} PersonName;

bool validName(char *str);

/**
 * Check the input is valid or not
 * return True/False
 * use Regular expression
 */
bool validName(char *str){
	// char *regex_pattern = "^[A-Z](['-][A-Z])?[a-z]+([ -]?[A-Z][a-z]+)*,[ ]?[A-Z][a-z]+([ ][A-Z][a-z]+)*$";
	char *regex_pattern = "^[A-Z][A-Za-z'-]+([ ][A-Z][A-Za-z'-]+)*,[ ]?[A-Z][A-Za-z'-]+([ ][A-Z][A-Za-z'-]+)*$";
	regex_t regex;
	int flag;
	bool result = false;
	flag = regcomp(&regex, regex_pattern, REG_EXTENDED | REG_NOSUB);
	if (flag == 0) {
		if (!regexec(&regex, str, 0, NULL, 0)){
			result = true;
		}
	}
	regfree(&regex);
	return result;
}

/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/
PG_FUNCTION_INFO_V1(pname_in);

Datum
pname_in(PG_FUNCTION_ARGS)
{
	char *str = PG_GETARG_CSTRING(0);
	PersonName *result;
	int length;
	// to check whether input is valid
	if (!validName(str))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for type %s: \"%s\"",
						"PersonName", str)));
	// plus 1 for '\0'
	length = strlen(str) + 1;
	result = (PersonName *) palloc(VARHDRSZ + length);
	SET_VARSIZE(result, VARHDRSZ + length);
	snprintf(result->name, length , "%s", str);
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *result;
	//delete the space after comma when show the result of select queries the space should be ignored
	char *given;
	given = strchr(pname->name, ',');
	int f_len = strlen(pname->name) - strlen(given);
	given++;
	if (*given == ' ') {
		given++;
	}
	pname->name[f_len] = '\0';
	result = psprintf("%s,%s", pname->name, given);
	pname->name[f_len] = ',';
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
	const char *personname = pq_getmsgstring(buf);
	int length = strlen(personname) + 1;
	result = (PersonName *) palloc(VARHDRSZ + length);
	SET_VARSIZE(result, VARHDRSZ + length);
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

static int pname_cmp_internal(PersonName * a, PersonName * b)
{	
	int a_divide, b_divide, result;
	char *a_given, *b_given;
	for (a_divide = 0; a_divide < strlen(a->name); a_divide++) {
		if (a->name[a_divide] == ',') {
			break;
		}
	}
	for (b_divide = 0; b_divide < strlen(b->name); b_divide++) {
		if (b->name[b_divide] == ',') {
			break;
		}
	}
	a_given = &a->name[a_divide + 1];
	b_given = &b->name[b_divide + 1];
	// a_given = strchr(a->name, ',');
	// b_given = strchr(b->name, ',');
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

PG_FUNCTION_INFO_V1(pname_cmp);

Datum
pname_cmp(PG_FUNCTION_ARGS)
{
	PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
	PersonName *b = (PersonName *) PG_GETARG_POINTER(1);
	PG_RETURN_INT32(pname_cmp_internal(a, b));
}

PG_FUNCTION_INFO_V1(family);

Datum
family(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *name;
	text *result;
	int length;
	int divide;
	for (divide = 0; divide < strlen(pname->name); divide++) {
		if (pname->name[divide] == ',') {
			break;
		}
	}
	pname->name[divide] = '\0';
	name = psprintf("%s", pname->name);
	pname->name[divide] = ',';
	length = strlen(name);
	result = (text *) palloc(VARHDRSZ + length);
	SET_VARSIZE(result, VARHDRSZ + length);
	memcpy(VARDATA(result), name, length);
	// return text type in order to do order by operation
	PG_RETURN_TEXT_P(result);
}

PG_FUNCTION_INFO_V1(given);

Datum
given(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	text *result;
	char *name;
	char *given;
	int length;
	given = strrchr(pname->name, ',');
	given++;
	if (*given == ' ') {
		given++;
	}
	name = psprintf("%s", given);
	length = strlen(name);
	result = (text *) palloc(VARHDRSZ + length);
	SET_VARSIZE(result, VARHDRSZ + length);
	memcpy(VARDATA(result), name, length);
	// return text type in order to do order by operation
	PG_RETURN_TEXT_P(result);
}

PG_FUNCTION_INFO_V1(show);

Datum
show(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	char *name;
	char *given;
	text *result;
	char *more_given;
	int length;
	int f_len = 0, g_len = 0;
	given = strchr(pname->name, ',');
	f_len = strlen(pname->name) - strlen(given);
	given++;
	if (*given == ' ') {
		given++;
	}
	more_given = strchr(given, ' ');
	pname->name[f_len] = '\0';
	if (!more_given) {
		name = psprintf("%s %s", given, pname->name);
	}
	else {
		// compute length of the first part given name
		g_len = strlen(given) - strlen(more_given);
		*(given + g_len) = '\0';
		name = psprintf("%s %s", given, pname->name);
		*(given + g_len) = ' ';
	}
	pname->name[f_len] = ',';
	length = strlen(name);
	result = (text *) palloc(VARHDRSZ + length);
	SET_VARSIZE(result, VARHDRSZ + length);
	memcpy(VARDATA(result), name, length);
	// return text type in order to do order by operation
	PG_RETURN_TEXT_P(result);
}

PG_FUNCTION_INFO_V1(pname_hash);

Datum
pname_hash(PG_FUNCTION_ARGS)
{
	PersonName *pname = (PersonName *) PG_GETARG_POINTER(0);
	
	// delete the space after comma if have
	int divide;
	char *given_part;
	int h_code;
	for (divide = 0; divide < strlen(pname->name); divide++) {
		if (pname->name[divide] == ',') {
			break;
		}
	}
	given_part = strrchr(pname->name, ',');
	given_part++;
	if (*given_part == ' ') {
		given_part++;
		divide++;
		while(*given_part != '\0') {
			pname->name[divide] = *given_part;
			given_part++;
			divide++;
		}
		pname->name[divide] = *given_part;
	}
	h_code = DatumGetUInt32(hash_any((unsigned char *) pname->name, strlen(pname->name)));
	
	PG_RETURN_INT32(h_code);
}
