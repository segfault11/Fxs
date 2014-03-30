//
//  main.c
//  ObjMeshTest
//
//  Created by Arno in Wolde Luebke on 25.03.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include <Fxs/Obj/ObjMesh.h>

int main(int argc, const char * argv[])
{
    FxsObjMesh* mesh = NULL;
    FxsObjMeshCreateWithFile(&mesh, "LighthouseTerrain.obj");
    FxsObjMeshDestroy(&mesh);
    
    printf("Hello, World!\n");
    return 0;
}

