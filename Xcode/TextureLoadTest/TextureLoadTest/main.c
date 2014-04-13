//
//  main.c
//  TextureLoadTest
//
//  Created by Arno in Wolde Luebke on 08.04.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include <FF/MainLoop/MainLoop.h>
#include "TextureLoadTest.h"

int main(int argc, const char * argv[])
{
    FFMainLoopCreate(NULL);
    FFMainLoopSetInitFunc(Init);
    FFMainLoopSetUpdateFunc(Update);
    FFMainLoopSetFinalizeFunc(Finalize);
    FFMainLoopRun();
    FFMainLoopDestroy();

    return 0;
}

