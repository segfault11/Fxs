//
//  main.c
//  DictionaryTest
//
//  Created by Arno in Wolde Luebke on 30.03.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "Dictionary.h"

void Test01()
{
    FxsDictionaryPtr d = FxsDictionaryCreateWithTableSize(100);
    FxsListIteratorPtr it = NULL;
    FxsListPtr keys = NULL;
    const char* key = NULL;
    
    FxsDictionaryInsert(d, "a", NULL);
    FxsDictionaryInsert(d, "b", NULL);
    FxsDictionaryInsert(d, "c", NULL);
    FxsDictionaryInsert(d, "d", NULL);
    FxsDictionaryRemove(d, "c");
    FxsDictionaryInsert(d, "a", NULL);
    
    keys = FxsDictionaryGetKeys(d);
    it = FxsListIteratorCreate(keys, FXS_LIST_FRONT, FXS_LIST_FRONT_TO_BACK);
    
    printf("# keys: %zu\n", FxsListGetSize(keys));
    
    while (FxsListIteratorHasNext(it))
    {
        key = FxsListIteratorNext(it);
        printf("%s\n", key);
    }

    FxsListIteratorDestroy(&it);
    FxsDictionaryDestroy(&d);
}

void Test02()
{
    char* test = "test";
    FxsDictionaryPtr d = FxsDictionaryCreateWithTableSize(1);

    FxsDictionaryInsert(d, "1", test);
    
    printf("size %zu\n", FxsDictionaryGetSize(d));
    
    
    FxsDictionaryRemove(d, "bla");

    printf("size %zu\n", FxsDictionaryGetSize(d));

    FxsDictionaryDestroy(&d);
}

int main(int argc, const char * argv[])
{
    Test02();

    return 0;
}

