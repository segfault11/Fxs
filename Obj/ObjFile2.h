/*
 * A more simple and easier to definition of an .obj file 
 * (compared to ObjFile.h)
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
#ifndef OBJFILE2_H
#define OBJFILE2_H

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

#define FXS_OBJ_FILE2_MAX_STRLEN 64

enum {
	FXS_OBJ2_FACE_NORMALS = 1,
	FXS_OBJ2_FACE_TEXCOORDS = 2,
};

/*
** Representation of a face. Holds indices to position, normal and texture
** coordinates in the "FxsObjFile2" structure.
*/ 
typedef struct {
	int type;
	int p0, p1, p2;
	int n0, n1, n2;
	int tc0, tc1, tc2;
} FxsObjFile2Face;

typedef struct {
	char name[FXS_OBJ_FILE2_MAX_STRLEN];
	char materialName[FXS_OBJ_FILE2_MAX_STRLEN];
	int childId;
	unsigned int numChilds;
	int faceId;
	unsigned int numFaces;
} FxsObjFile2Node;

typedef struct  {
	unsigned int numPostitions;
	unsigned int numNormals;
	unsigned int numTexCoords;
	unsigned int numFaces;
	unsigned int numNodes;

    FxsVector3 *positions;
    FxsVector3 *normals;
    FxsVector2 *texCoords;

	FxsObjFile2Face *faces;
	FxsObjFile2Node *nodes;

	char mtlFilename[FXS_OBJ_FILE2_MAX_STRLEN];
} FxsObjFile2;

FxsObjFile2* FxsObjFile2Create(const char *filename);
void FxsObjFile2Destroy(FxsObjFile2 **file);

#endif /* end of include guard: OBJFILE2_H */
