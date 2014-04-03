/*
 * Simple Implementation of a dictionary
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
#ifndef DICTIONARY_H
#define DICTIONARY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include "../List/List.h"

typedef struct FxsDictionary_* FxsDictionaryPtr;

/*
** Creates a dictionary with a given table size.
**
** @param tableSize The table size of the hash table used by the dictionary
** @return NULL if the creation failed, otherwise a pointer to the dictionary.
*/ 
FxsDictionaryPtr FxsDictionaryCreateWithTableSize(size_t tableSize);

/*
** Inserts a pointer (value) to the dictionary. This method does not copy the 
** data the pointer references. If the key already exists in the dictionary
** this method does nothing.
**
** @param dict The dictionary the value is stored in.
** @param key The key the pointer is assiciated with.
** @param value The pointer to be stored.
**
** @return 0 if the method fails, 1 otherwise.
*/ 
int FxsDictionaryInsert(FxsDictionaryPtr dict, const char* key, void* value);

/*
** Checks if the dictionary contains a value for a given key.
**
** @param dict Dictionary that we want to check.
** @param key The key we want to check for.
**
** @return 1 if the dictionary contains the key (and hense the value), 
**         0 otherwise.
*/ 
int FxsDictionaryContains(FxsDictionaryPtr dict, const char* key);

/*
** Looks up a pointer for a key.
**
** @param dict Dictionary we look up from.
** @param key Key for the value we would like to retrive.
** 
** @return NULL if the corresponding value could not be found, the value
**  			otherwise.
*/ 
void* FxsDictionaryFind(FxsDictionaryPtr dict, const char* key);

/*
** Removes an entry from the dictionary. 
**
** @param dict The dictionary the entry should be removed from.
** @param key The key referencing the entry.
**
** @return 1 if the entry could be removed, 0 if the entry was not found.
*/ 
int FxsDictionaryRemove(FxsDictionaryPtr dict, const char* key);


/*
** Releases the dictionary. Note that the entries the dictionary are not managed
** by the dictionary and should be freed manually.
**
** @param dict The dictionary to be released.
*/ 
void FxsDictionaryDestroy(FxsDictionaryPtr* dict);


/*
** Returns a list of all keys that are currently inside the dictionary. 
** Note: The list is maintained by the dictionary, do not release it manually.
**       However, if you request an iterator from the list it should be 
**       released manually.
**
** @param dict The dictionary we want the list from.
** @return A pointer to the keys list.
*/ 
FxsListPtr FxsDictionaryGetKeys(FxsDictionaryPtr dict);


size_t FxsDictionaryGetSize(FxsDictionaryPtr dict);


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: DICTIONARY_H */
