#include "Log.h"
#include "../Array/Array.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

struct FxsLog {
	FxsArray *entries;
	unsigned int entryLen;	
};

FxsLogPtr FxsLogCreate(unsigned int entryLen)
{
	FxsLogPtr lp = malloc(sizeof(struct FxsLog));

	if (!lp) {
	    return NULL;
	}

	lp->entryLen = entryLen;
	lp->entries = FxsArrayCreate(1, sizeof(char*));

	if (!lp->entries) {
		free(lp);
	    return NULL;
	}

	return lp;
}

void FxsLogPrintf(FxsLogPtr log, const char *format, ...)
{
	char *entry = malloc(log->entryLen * sizeof(char));
	va_list args;

	if (!entry) {
	    return;
	}

	va_start (args, format);
	vsnprintf (entry, log->entryLen, format, args);
	va_end (args);

	FxsArrayPush(log->entries, &entry);
}

unsigned int FxsLogGetNumEntries(FxsLogPtr log)
{
	return (unsigned int)log->entries->count;
}

const char* FxsLogGetEntry(FxsLogPtr log, unsigned int entry)
{
	if (entry >= log->entries->count) {
	    return NULL;
	}

	return *((char**)(log->entries->data + log->entries->elementSize * entry));
}

void FxsLogDestroy(FxsLogPtr *log)
{
	unsigned int i = 0;

	if (!log || !(*log)) {
	    return;
	}

	FxsLogPtr l = *log;

	for (i = 0; i < l->entries->count; i++) {
	    free(*((char**)(l->entries->data + l->entries->elementSize * i)));
	}

	free(l);
	*log = NULL;
}
