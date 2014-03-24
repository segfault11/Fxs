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

typedef struct FxsDictionary_* FxsDictionaryPtr;


int FxsDictionaryCreateWithTableSize(FxsDictionaryPtr* dict, size_t tableSize);



/*
** Inserts a pointer (value) to the dictionary. This method copies the
** data the pointer references. The copy of the data is managed by the 
** dictionary. Do not free it, once you got access to this data via
** FxsDictionaryFind. If the key already exists in the dictionary this 
** method does nothing.
**
** @param dict The dictionary the value is stored in.
** @param key The key the pointer is assiciated with.
** @param value The pointer to the data to be stored.
** @param size Size of the datae the value pointer points to.
**
** @return 0 if the method fails, 1 otherwise.
*/ 
int FxsDictionaryInsert(FxsDictionaryPtr dict, const char* key, void* value, size_t size);


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
int FxsDictionaryInsertReference(FxsDictionaryPtr dict, const char* key, void* value);

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

int FxsDictionaryRemove(FxsDictionaryPtr dict, const char* key);


void FxsDictionaryDestroy(FxsDictionaryPtr* dict);

 


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: DICTIONARY_H */
