#include "Number.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_STR_LENGTH 256

/*
** Currently, the internal representation for a number is a string.
*/ 
typedef struct FxsNumber_
{
	char number[MAX_STR_LENGTH];
}
FxsNumber;

FxsNumberPtr FxsNumberCreateWithInt(int i)
{
	FxsNumber* n = (FxsNumber*)malloc(sizeof(FxsNumber));

	if (!n) 
	{
	    return NULL;
	}

	sprintf(n->number, "%i", i);

	return n;
}

FxsNumberPtr FxsNumberCreateWithFloat(float f)
{
	FxsNumber* n = (FxsNumber*)malloc(sizeof(FxsNumber));

	if (!n) 
	{
	    return NULL;
	}

	sprintf(n->number, "%f", f);

	return n;
}

void FxsNumberDestroy(FxsNumberPtr* n)
{
	if (!n || !(*n)) 
	{
	    return;
	}

	free(*n);
	n = NULL;
}

int FxsNumberAsInt(FxsNumberPtr n)
{
	return atoi(n->number);
}

float FxsNumberAsFloat(FxsNumberPtr n)
{
	return atof(n->number);
}

const char* FxsNumberAsString(FxsNumberPtr n)
{
	return n->number;
}
