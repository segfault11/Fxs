#include "Array.h"
#include <memory.h>
#include <stdio.h>

FxsArray* FxsArrayCreate(size_t maxElements, size_t elementSize)
{
	FxsArray *a = calloc(1, sizeof(FxsArray));

	if (!a) {
	    return 0;
	}

	a->grow = 2;
	a->elementSize = elementSize;
	a->count = 0;
	a->max = maxElements;
	a->data = calloc(a->max, elementSize);

	if (!a->data) {
	    free(a);
		return NULL;
	}

	return a;
}

int FxsArrayPush(FxsArray *array, const void *element)
{
	void *data = NULL;

	if (array->max == array->count) {
	    data = realloc(array->data,
            array->max * array->grow * array->elementSize);
		
		if (!data) {
		   	return 0; 
		}

		array->data = data;
		array->max *= array->grow;
	}

	memcpy(array->data + array->elementSize * array->count, element, array->elementSize);
	array->count++;
	return 1;
}

void FxsArrayDestroy(FxsArray **array)
{
	FxsArray *a;

	if (!array || !(*array)) {
	    return;
	}

	a = *array;

	free(a->data);
	free(a);
}
