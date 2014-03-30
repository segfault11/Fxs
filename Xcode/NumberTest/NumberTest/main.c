//
//  main.c
//  NumberTest
//
//  Created by Arno in Wolde Luebke on 26.03.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "Number.h"

void Test01()
{
    float f = 1343.123456789;
    FxsNumberPtr n = FxsNumberCreateWithFloat(f);
    
    printf("\n\nTest01:\n\n");
    
    printf("number %f\n", f);
    printf("number as int %d\n", FxsNumberAsInt(n));
    printf("number as float %f\n", FxsNumberAsFloat(n));
    printf("number as string %s\n", FxsNumberAsString(n));
    
    FxsNumberDestroy(&n);
}

void Test02()
{
    int i = 42;
    FxsNumberPtr n = FxsNumberCreateWithInt(i);
    
    printf("\n\nTest02:\n\n");
    
    printf("number %d\n", i);
    printf("number as int %d\n", FxsNumberAsInt(n));
    printf("number as float %f\n", FxsNumberAsFloat(n));
    printf("number as string %s\n", FxsNumberAsString(n));
    
    FxsNumberDestroy(&n);
}

int main(int argc, const char * argv[])
{
    Test01();
    Test02();

    printf("Hello, World!\n");
    return 0;
}

