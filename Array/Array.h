/*
 * A dynamically growing array
 * Copyright (C) 2014 Arno in Wolde Luebke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ARRAY_H
#define ARRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

typedef struct  {
    char *data;
	size_t count;
	size_t max;
	size_t elementSize;
	unsigned int grow;
} FxsArray;

FxsArray* FxsArrayCreate(size_t maxElements, size_t elementSize);
int FxsArrayPush(FxsArray *array, const void *element); 
void FxsArrayDestroy(FxsArray **array);


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: ARRAY_H */
