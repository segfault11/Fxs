//
//  main.c
//  ObjBoundingBox
//
//  Created by Arno in Wolde Luebke on 16.04.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "ObjFile.h"
#include <float.h>
#include <math.h>

int main(int argc, const char * argv[])
{
    FxsObjFilePtr objFile = NULL;
    unsigned int numPositions = 0;
    FxsVector3 min, max, pos;
    int  i = 0;

    if (argc == 1)
    {
        printf("usage: %s <filename>", argv[0]);
        return EXIT_FAILURE;
    }

    objFile = FxsObjFileCreateWithFile(argv[1]);
    
    if (!objFile)
    {
        printf("file could not be loaded.");
        return EXIT_FAILURE;
    }
    
    numPositions = FxsObjFileGetNumPositions(objFile);
    
    if (!numPositions)
    {
        printf("%s contains no positions", argv[1]);
    }
    
    min.x = min.y = min.z = FLT_MAX;
    max.x = max.y = max.z = FLT_MIN;
    
    for (i = 0; i < numPositions; i++)
    {
        FxsObjFileGetPosition(objFile, &pos, i);
        
        min.x = fminf(min.x, pos.x);
        min.y = fminf(min.y, pos.y);
        min.z = fminf(min.z, pos.z);

        max.x = fmaxf(max.x, pos.x);
        max.y = fmaxf(max.y, pos.y);
        max.z = fmaxf(max.z, pos.z);
    }
    
    printf("min = [%f %f %f]\n", min.x, min.y, min.z);
    printf("max = [%f %f %f]\n", max.x, max.y, max.z);
    
    FxsObjFileDestroy(&objFile);

    return EXIT_SUCCESS;
}

