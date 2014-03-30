#include "Dictionary.h"
#include <memory.h>
#include <string.h>

struct FxsDictionaryBucket_;

typedef struct FxsDictionaryBucket_
{
	FxsListIteratorPtr it;             /* stores the position of the key for 
									   ** this bucket element in the keys list, 
									   ** so we can remove the key from the 
									   ** list in O(1).
									   */
	struct FxsDictionaryBucket_* next;
	char* key;
	void* value;
}
FxsDictionaryBucket;

typedef struct FxsDictionary_
{
	size_t tableSize;
	size_t size; 		/* # of entries in the dictionary */
	FxsDictionaryBucket** buckets;
	FxsListPtr keys;
}
FxsDictionary;

/*
** Computes the hash for a string according to the djb2 algorithm. (see:
** http://www.cse.yorku.ca/~oz/hash.html)
*/ 
size_t ComputeHash(const char *str)
{
    unsigned long hash = 5381;
    char c;

    while (1)
	{
		c = *str++;
		
		if (c == '\0')
		{
			break;
		}

        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

    return hash;
}


FxsDictionaryPtr FxsDictionaryCreateWithTableSize(size_t tableSize)
{
	FxsDictionary* d = (FxsDictionary*)malloc(sizeof(FxsDictionary));

	if (!d || !tableSize)
	{
		return NULL;
	}

	d->tableSize = tableSize;
	d->buckets = malloc(sizeof(FxsDictionaryBucket*)*tableSize);

	if (!d->buckets) 
	{
		free(d);
	    return NULL;
	}

	memset(d->buckets, 0, sizeof(FxsDictionaryBucket*)*tableSize);

	d->size = 0;
	d->keys = FxsListCreate();

	if (!d->keys) 
	{
	    free(d->buckets);
		free(d);
	}
	
	return d;
}

int FxsDictionaryInsert(FxsDictionaryPtr dict, const char* key, void* value)
{
	size_t hash = 0;
	FxsDictionaryBucket* prev = NULL;
	FxsDictionaryBucket* bucket = NULL;
	size_t strl;

	if (!dict || !key || !dict->tableSize) 
	{
	    return 0;
	}

	/* check if the key already exists */
	if (FxsDictionaryContains(dict, key))
	{
		return 0;
	}

	hash = ComputeHash(key) % dict->tableSize;

	bucket = dict->buckets[hash];

	if (bucket == NULL) /* if this is the first entry for the bucket */
	{
	    bucket = (FxsDictionaryBucket*)malloc(sizeof(FxsDictionaryBucket));
        dict->buckets[hash] = bucket;
	}
	else /* else search for the last entry in this bucket ... */
	{
		while (1)
		{
			if (!bucket->next) 
			{
				/* .. attach a new entry and break */
				bucket->next = (FxsDictionaryBucket*)malloc(
						sizeof(FxsDictionaryBucket)
					);

                prev = bucket;
				bucket = bucket->next;

		    	break;
			}

			bucket = bucket->next;
		}
	}

	/* complain if malloc failed */
	if (!bucket)
	{
		return 0;
	}

	strl = strlen(key + 1);
	bucket->key = (char*)malloc(strl);

	if (!bucket->key) 
	{
        if (prev)
        {
            prev->next = NULL;
        }
		else
		{
			dict->buckets[hash] = NULL;
		}
        
	    free(bucket);
		return 0;
	}

	strcpy(bucket->key, key);
	bucket->value = value;
	bucket->next = NULL;

	/* push back the key to the list that manages all our keys */
	if (!FxsListPushBack(dict->keys, bucket->key))
	{
		free(bucket->key);
	
        if (prev)
        {
            prev->next = NULL;
        }
		else
		{
			dict->buckets[hash] = NULL;
		}
        
	    free(bucket);
		return 0;
	}
	else
	{
		bucket->it = FxsListIteratorCreate(dict->keys, FXS_LIST_BACK, FXS_LIST_BACK_TO_FRONT);
		FxsListIteratorNext(bucket->it);
	}

    dict->size++;

	return 1;	
}

int FxsDictionaryRemove(FxsDictionaryPtr dict, const char* key)
{
	size_t hash = 0;
	FxsDictionaryBucket* prevBucket = NULL;
	FxsDictionaryBucket* bucket = NULL;

	if (!dict || !key || !dict->tableSize) 
	{
	    return 0;
	}

	hash = ComputeHash(key) % dict->tableSize;

	bucket = dict->buckets[hash];

	/* case 0: zero entries for bucket */
	if (!bucket) 
	{
	    return 0;
	}

	/* case 1: first entry corresponds to the key */
	if (!strcmp(bucket->key, key)) 
	{
		dict->buckets[hash] = bucket->next;

		FxsListIteratorRemove(bucket->it);
		FxsListIteratorDestroy(&bucket->it);		
		free(bucket->key);
		free(bucket);

        dict->size--;

		return 1;
	}

	/* case 2: look for the following entries .. */
	while (bucket)
	{
		if (!strcmp(bucket->key, key)) /* rem. if you find a corresponding key */
		{
			prevBucket->next = bucket->next;
            
			FxsListIteratorRemove(bucket->it);
			FxsListIteratorDestroy(&bucket->it);		
			free(bucket->key);
			free(bucket);	
			
			dict->size--;
			
		    return 1;
		}
		
		prevBucket = bucket;
		bucket = bucket->next;
	}

	return 0;	
}

int FxsDictionaryContains(FxsDictionaryPtr dict, const char* key)
{
	size_t hash = 0;
	FxsDictionaryBucket* bucket = NULL;

	if (!dict || !key || !dict->tableSize) 
	{
	    return 0;
	}

	hash = ComputeHash(key) % dict->tableSize;

	bucket = dict->buckets[hash];

	while (bucket)
	{
		if (!strcmp(bucket->key, key))
		{
		    return 1;
		}
		
		bucket = bucket->next;
	}

	return 0;
}

void* FxsDictionaryFind(FxsDictionaryPtr dict, const char* key)
{
	size_t hash = 0;
	FxsDictionaryBucket* bucket = NULL;

	if (!dict || !key || !dict->tableSize) 
	{
	    return 0;
	}

	hash = ComputeHash(key) % dict->tableSize;

	bucket = dict->buckets[hash];

	while (bucket)
	{
		if (!strcmp(bucket->key, key)) /* rem. if you find a corresponding key */
		{
		    return bucket->value;
		}
		
		bucket = bucket->next;
	}


	return NULL;
}

static void DeleteBucket(FxsDictionaryBucket* bucket)
{
	/* delete following entries first if they exist! */
	if (bucket->next) 
	{
	    DeleteBucket(bucket->next);
	}

	free(bucket->key);
	free(bucket);
}

void FxsDictionaryDestroy(FxsDictionaryPtr* dict)
{
	int i = 0;

	if (!dict || !(*dict)) 
	{
	   	return; 
	}

	for (i = 0; i < (*dict)->tableSize; i++)
	{
		if ((*dict)->buckets[i])
		{
			DeleteBucket((*dict)->buckets[i]);
		}
	}

	free((*dict)->buckets);
	FxsListDestroy(&(*dict)->keys);
	free(*dict);

	dict = NULL;
}

FxsListPtr FxsDictionaryGetKeys(FxsDictionaryPtr dict)
{
	if (!dict) 
	{
	    return NULL;
	}

	return dict->keys;
}
