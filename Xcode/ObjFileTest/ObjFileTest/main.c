//
//  main.c
//  ObjFileTest
//
//  Created by Arno in Wolde Luebke on 03.04.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include "ObjFile.h"

int main(int argc, const char * argv[])
{
    FxsObjFilePtr file = NULL;
    FxsVector3 position;
    FxsVector3 normal;
    FxsVector2 texCoord;
    FxsObjFace* face;
    FxsObjObjectPtr object;
    FxsObjGroupPtr group;
    FxsListPtr objects;
    FxsListIteratorPtr objectsIterator;
    FxsListIteratorPtr groupIterator;
    FxsListIteratorPtr faceIterator;
    unsigned int numPositions = 0;
    unsigned int numNormals = 0;
    unsigned int numTexCoords = 0;
    int i = 0;
    
    file = FxsObjFileCreate("LighthouseColored.obj");
    
    if (!file)
    {
        printf("failed to load\n");
        return 0;
    }
    
    numPositions = FxsObjFileGetNumPositions(file);
    
    for (i = 0; i < numPositions; i++)
    {
        FxsObjFileGetPosition(file, &position, i);
        printf("v %f %f %f\n", position.x, position.y, position.z);
    }
    
    numNormals = FxsObjFileGetNumNormals(file);
    
    for (i = 0; i < numNormals; i++)
    {
        FxsObjFileGetNormal(file, &normal, i);
        printf("vn %f %f %f\n", normal.x, normal.y, normal.z);
    }
    
    numTexCoords = FxsObjFileGetNumTexCoords(file);
    
    for (i = 0; i < numTexCoords; i++)
    {
        printf("vt %f %f", texCoord.x, texCoord.y);
    }
    
    objects = FxsObjFileGetObjects(file);
    objectsIterator = FxsListIteratorCreate(objects, FXS_LIST_FRONT, FXS_LIST_FRONT_TO_BACK);
    
    while (FxsListIteratorHasNext(objectsIterator))
    {
        object = FxsListIteratorNext(objectsIterator);
        printf("o %s\n", FxsObjObjectGetName(object));
        
        groupIterator = FxsListIteratorCreate(
                FxsObjObjectGetGroups(object),
                FXS_LIST_FRONT,
                FXS_LIST_FRONT_TO_BACK
            );
        
        while (FxsListIteratorHasNext(groupIterator))
        {
            group = FxsListIteratorNext(groupIterator);
            printf("g %s\n", FxSObjGroupGetName(group));
            
            faceIterator = FxsListIteratorCreate(FxsObjGroupGetFaces(group), FXS_LIST_FRONT, FXS_LIST_FRONT_TO_BACK);
            
            while (FxsListIteratorHasNext(faceIterator))
            {
                face = FxsListIteratorNext(faceIterator);
                printf(
                    "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                    face->p0, face->tc0, face->n0,
                    face->p1, face->tc1, face->n1,
                    face->p2, face->tc2, face->n2
                );
            }
            
            FxsListIteratorDestroy(&faceIterator);
        }
        
        FxsListIteratorDestroy(&groupIterator);
    }
    
    FxsObjFileDestroy(&file);
    
    return 0;
}

