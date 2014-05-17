//
//  main.c
//  LogTest
//
//  Created by Arno in Wolde Luebke on 17.05.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "Log.h"

void Test00()
{
    FxsLogPtr l = FxsLogCreate(100);
    
    FxsLogPrintf(l, "Hello %f", 42.0f);
    FxsLogPrintf(l, "Hello %f %d", 46.0f, 12);

    printf("%s\n", FxsLogGetEntry(l, 0));
    printf("%s\n", FxsLogGetEntry(l, 1));
    
    FxsLogDestroy(&l);
}

int main(int argc, const char * argv[])
{
    Test00();
    return 0;
}

