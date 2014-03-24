/*
 * Loads an .obj file
 * Copyright (C) 2014 Arno Luebke
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OBJFILE_H
#define OBJFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <Fxs/Math/Vector2.h>
#include <Fxs/Math/Vector3.h>

enum
{
    FXS_OBJ_ROOT = 0,
    FXS_OBJ_OBJECT,
    FXS_OBJ_GROUP
};

/*
** Representation of a face.
** Stores indices to vertex, normal and tex coord data and a index to the name
** of a material file for a face
*/ 
typedef struct
{
	int p0, p1, p2;    	/* position indices */
	int n0, n1, n2; 	/* normal indices */
	int tc0, tc1, tc2;  /* texcord indices */
	int matIdx; 		/* index to the name of a material file, -1 if no
						** material was assigned to this face
						*/
}
FxsObjFace; 

typedef struct
{
    int type; 				/* type of the node either FXS_OBJ_GROUP or 
							** FXS_OBJ_OBJECT */
    char* name;             /* name of the node */
    int faceId;             /* index to the first face this node references, 
                            ** -1 if the node has no faces.
                            */
    int numFaces;           /* # of faces this node references*/
    int childId; 			/* index to the first child this node references,
							** -1 if the node has no children
							*/
    int numChildren;        /* # of children this node holds */
}
FxsObjNode;

typedef struct
{
	char* name;                 /* name of the obj file */
    char* mtlname;              /* name of the .mtl file */

    char** materials;           /* names of the materials */
    FxsObjFace* faces; 			
    FxsObjNode* nodes;
	FxsVector3* positions;
	FxsVector3* normals;
	FxsVector2* texCoords;

    unsigned int numMaterials;
    unsigned int numFaces;
	unsigned int numPositions;
	unsigned int numNormals;
	unsigned int numTexCoords;
	unsigned int numNodes;
    
    FxsObjNode root;
}
FxsObjMesh;

int FxsObjMeshCreateWithFile(FxsObjMesh** mesh, const char* filename); 
void FxsObjMeshDestroy(FxsObjMesh** mesh);



#ifdef __cplusplus
}
#endif


#endif /* end of include guard: OBJFILE_H */
