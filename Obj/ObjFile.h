/*
 * Loads an .obj file
 * Copyright (C) 2014 Arno in Wolde Luebke
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

#include <stdlib.h>
#include "../List/List.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

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

typedef struct FxsObjGroup_* FxsObjGroupPtr;
typedef struct FxsObjObject_* FxsObjObjectPtr;

/*
** Representation of an .obj file.
**
** Some notes:
**  	- While loading the .obj file always a default object (FxsObjObject)
**        is created. The name of the default object is the NULL ptr. If the
**   	  to be loaded .obj file has "o" definitions from the beginning the
**        default object will be empty.
** 		- Each default object has a default group (FxsObjGroup). The name of
** 		  the group is the NULL ptr. If a "g" is given from beginning in the 
**        .obj file the default group is empty.
*/ 
typedef struct FxsObjFile_* FxsObjFilePtr;

/*
** Gets a list of faces that belong to the group. The list and its contents
** are managed by the FxsObjFile data structure do not release it.
**
** @param group The group we wish to get the faces for.
** @return A list the consists of pointers to the faces that the group contains.
**         A face is represented by the FxsObjFace data structure.
*/ 
FxsListPtr FxsObjGroupGetFaces(FxsObjGroupPtr group);

/*
** Returns the name of a group or NULL if it is a default group.
**
** @param group Group, we want the name for.
** @return Name of the group.
*/ 
const char* FxSObjGroupGetName(FxsObjGroupPtr group);

/*
** Gets the group(s) that are containted in an Object in form of a list.
** The list stores FxsObjGroupPtr's. The list and its contents
** are managed by the FxsObjFile data structure do not release it.
**
** @param object The object we wish to get the groups for.
** @param A list that stores FxsObjGroup's.
*/ 
FxsListPtr FxsObjObjectGetGroups(FxsObjObjectPtr object);

/*
** Returns the name of the object or NULL if it is the default object.
**
** @param object The object that we want the name for.
** @return The name of the object.
*/ 
const char* FxsObjObjectGetName(FxsObjObjectPtr object);

/*
** Gets the object's contained in an FxsObjFile data structure, in form of a
** List that stores FxsObjObjectPtr's. The list and its contents
** are managed by the FxsObjFile data structure do not release it.
**
** @param file The object file we wish to retrieve the objects for.
** @return A list pof FxsObjObjectPtr's.
*/ 
FxsListPtr FxsObjFileGetObjects(FxsObjFilePtr file);

/*
** Loads an .obj file from storage and creates a FxsObjFile struct for it.
**
** @param filename The filename of the .obj file we wish to load, as a C ptr.
** @return A FxsObjFilePtr or NULL of loading the file failed.
*/ 
FxsObjFilePtr FxsObjFileCreateWithFile(const char* filename);

/*
** Destroys an FxsObjFile and all resources associated with it.
**
** @param objFile The FxsObjFile to be released.
*/ 
void FxsObjFileDestroy(FxsObjFilePtr* objFile);

/*
** Gets the position with an id and stores it into "pos". Does nothing and 
** returns 0 if the position for the id could not be found (id >= numPos).
*/ 
int FxsObjFileGetPosition(FxsObjFilePtr file, FxsVector3* pos, unsigned int id);

/*
** Gets the normal with an id and stores it into "normal". Does nothing and 
** returns 0 if the normal for the id could not be found (id >= numNormals).
*/ 
int FxsObjFileGetNormal(FxsObjFilePtr file, FxsVector3* normal, unsigned int id);

/*
** Gets the texture coordinate with an id and stores it into "tc". Does nothing 
** and returns 0 if the tex coordinate for the id could not be found 
** (id >= numTexcoordinates).
*/ 
int FxsObjFileGetTexCoord(FxsObjFilePtr file, FxsVector2* tc, unsigned int id);

/*
** Gets the name of an material with and id. returns NULL if the material name
** for the id does not exist.
*/ 
const char* FxsObjFileGetMaterialName(FxsObjFilePtr file, unsigned int id);

/*
** Gets the .mtl file name associated with this .obj file.
*/ 
const char* FxsObjFileGetMtlName(FxsObjFilePtr file);

/*
** Gets the number of materials for the .obj file.
*/
unsigned int FxsObjFileGetNumMaterials(FxsObjFilePtr file);

/*
** Gets the number of positions contained in the file
*/ 
unsigned int FxsObjFileGetNumPositions(FxsObjFilePtr file);

/*
** Gets the number of tex coordinates contained in the file
*/ 
unsigned int FxsObjFileGetNumTexCoords(FxsObjFilePtr file);

/*
** Gets the number of normals contained in the file
*/ 
unsigned int FxsObjFileGetNumNormals(FxsObjFilePtr file);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: OBJFILE_H */
