//
//  main.c
//  ObjFile2Test
//
//  Created by Arno in Wolde Luebke on 18.05.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include <memory.h>
#include "ObjFile2.h"

#define STRLEN 64


void DumpNode(const FxsObjFile2 *obj, int id, int level)
{
    char pre[STRLEN];
    int i = 0;
    memset(pre, ' ', STRLEN);
    pre[level] = '\0';
    printf("%s+ %s / %s\n", pre, obj->nodes[id].name, obj->nodes[id].materialName);
    
    if (obj->nodes[id].childId == -1) {
        return;
    }

    for (i = 0; i < obj->nodes[id].numChilds; i++) {
        DumpNode(obj, obj->nodes[id].childId + i, level + 1);
    }
}

void DumpGraph(const FxsObjFile2 *obj)
{
    DumpNode(obj, 0, 0);
}

int main(int argc, const char * argv[])
{
    FxsObjFile2 *obj = FxsObjFile2Create("CornellBox-Original.obj");
    DumpGraph(obj);

//    printf("Hello, World!\n");
    return 0;
}

