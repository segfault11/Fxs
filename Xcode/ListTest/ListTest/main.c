//
//  main.c
//  ListTest
//
//  Created by Arno in Wolde Luebke on 26.03.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "List.h"
#include "Number.h"

void Test00()
{
    FxsListPtr list;
    FxsListIteratorPtr it;
    list = FxsListCreate();


    FxsListPushFront(list, "Hallo,");
    FxsListPushFront(list, "Welt!");

    it = FxsListIteratorCreate(list, FXS_LIST_BACK, FXS_LIST_BACK_TO_FRONT);
    
    while (FxsListIteratorHasNext(it))
    {
        printf("%s \n", FxsListIteratorNext(it));
    }

    FxsListDestroy(&list);
}

void Test01()
{
    int i = 0;
    
    FxsListPtr list = FxsListCreate();
    
    for (i = 0; i < 200; i++)
    {
        if(!FxsListPushBack(list, FxsNumberCreateWithInt(i)))
        {
            printf("push back failed!\n");
        }
    }
    printf("\n\tsize %zu \n", FxsListGetSize(list));
    FxsListIteratorPtr li = FxsListIteratorCreate(list, FXS_LIST_FRONT, FXS_LIST_FRONT_TO_BACK);
    
    while (FxsListIteratorHasNext(li))
    {
        int num = FxsNumberAsInt(FxsListIteratorNext(li));

        if (num % 2)
        {
            FxsListIteratorRemove(li);
        }
    }


    printf("\n\tsize %zu \n", FxsListGetSize(list));
    FxsListIteratorSetPosition(li, FXS_LIST_FRONT);
 
    while (FxsListIteratorHasNext(li))
    {
        int num = FxsNumberAsInt(FxsListIteratorNext(li));

        printf("%d\n", num);
    }
    printf("\n\tsize %zu \n", FxsListGetSize(list));
    FxsListIteratorSetPosition(li, FXS_LIST_BACK);
    FxsListIteratorSetDirection(li, FXS_LIST_BACK_TO_FRONT);

    while (FxsListIteratorHasNext(li))
    {
        FxsListIteratorNext(li);
        FxsListIteratorRemove(li);
    }
    
    FxsListIteratorSetPosition(li, FXS_LIST_FRONT);
    FxsListIteratorSetDirection(li, FXS_LIST_FRONT_TO_BACK);
    printf("\n\tsize %zu \n", FxsListGetSize(list));
 
    while (FxsListIteratorHasNext(li))
    {
        FxsListIteratorNext(li);
        FxsListIteratorRemove(li);
    }
    
    printf("\n\tsize %zu \n", FxsListGetSize(list));
    
    FxsListIteratorDestroy(&li);
    FxsListDestroy(&list);
}

int main(int argc, const char * argv[])
{
    Test01();

    return 0;
}

