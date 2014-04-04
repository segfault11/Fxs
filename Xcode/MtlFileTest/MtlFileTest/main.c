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
    FxsObjMaterialPtr mat = NULL;
    const char* attrElem = NULL;
    
    if (!file)
    {
        printf("could not load file\n");
        return 0;
    }
    
    mat = FxsObjMtlFileGetMaterialWithName(file, "Material.003");
    
    if (!mat)
    {
        printf("could not get material\n");
        FxsObjMtlFileDestroy(&file);
        return 0;
    }
    
    attrElem = FxsObjMaterialGetAttributeElementAsString(mat, "Ks", 2);
    
    if (!attrElem)
    {
        printf("could not get attr elem\n");
        FxsObjMtlFileDestroy(&file);
        return 0;
    }

    printf("Ns = %s\n", attrElem);
    
    FxsObjMtlFileDestroy(&file);
    
    return 0;
}

