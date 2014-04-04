//
//  main.c
//  MtlFileTest
//
//  Created by Arno in Wolde Luebke on 04.04.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "ObjMtlFile.h"

int main(int argc, const char * argv[])
{
    FxsObjMtlFilePtr file = FxsObjMtlFileCreateWithFile("LighthouseColored.mtl");
    
    if (!file)
    {
        printf("could not load file\n");
    }
    
    FxsObjMtlFileDestroy(&file);
    
    return 0;
}

