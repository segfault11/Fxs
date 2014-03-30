/*
 * A linked list
 * Copyright (C) 2014 Arno Luebke
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
#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

enum
{
	FXS_LIST_FRONT  = 0,
	FXS_LIST_BACK
};

enum
{
	FXS_LIST_FRONT_TO_BACK = 0,
	FXS_LIST_BACK_TO_FRONT
};

typedef struct FxsList_* FxsListPtr;
typedef struct FxsListIterator_* FxsListIteratorPtr;

FxsListPtr FxsListCreate();

int FxsListPushBack(FxsListPtr l, void* val);
int FxsListPushFront(FxsListPtr l, void* val);

void* FxsListGetBack(FxsListPtr l);
void* FxsListGetFront(FxsListPtr l);

void FxsListPopFront(FxsListPtr l);
void FxsListPopBack(FxsListPtr l);

size_t FxsListGetSize(FxsListPtr l);

void FxsListDestroy(FxsListPtr* l);


FxsListIteratorPtr FxsListIteratorCreate(FxsListPtr l, int pos, int dir);
void FxsListIteratorDestroy(FxsListIteratorPtr* li);

int FxsListIteratorSetPosition(FxsListIteratorPtr li, int pos);
int FxsListIteratorSetDirection(FxsListIteratorPtr li, int dir);
int FxsListIteratorHasNext(FxsListIteratorPtr li);
void* FxsListIteratorNext(FxsListIteratorPtr li);
void FxsListIteratorRemove(FxsListIteratorPtr li);


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: ARRAY_H */
