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
** Gets the group(s) that are containted in an Object in form of a list.
** The list stores FxsObjGroupPtr's. The list and its contents
** are managed by the FxsObjFile data structure do not release it.
**
** @param object The object we wish to get the groups for.
** @param A list that stores FxsObjGroup's.
*/ 
FxsListPtr FxsObjObjectGetGroups(FxsObjObjectPtr object);

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
** @param filename The filename of the .obj file we wish to load.
** @return A FxsObjFilePtr or NULL of loading the file failed.
*/ 
FxsObjFilePtr FxsObjFileCreate(const char* filename);

/*
** Destroys an FxsObjFile and all resources associated with it.
**
** @param objFile The FxsObjFile to be released.
*/ 
void FxsObjFileDestroy(FxsObjFilePtr* objFile);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: OBJFILE_H */
