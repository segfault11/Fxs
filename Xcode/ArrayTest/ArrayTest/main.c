//
//  main.c
//  ArrayTest
//
//  Created by Arno in Wolde Luebke on 17.05.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "Array.h"

void Test00()
{
    FxsArray *a = FxsArrayCreate(10, sizeof(int));
    int i = 0;
    int j;
    
    for (i = 0; i < 1000; i++) {
        j = i + 42;
        FxsArrayPush(a, &j);
    }

    for (i = 0; i < 1000; i++) {
        printf("%d\n", ((int*)a->data)[i]);
    }

    FxsArrayDestroy(&a);
}

int main(int argc, const char * argv[])
{
    Test00();
    return 0;
}

