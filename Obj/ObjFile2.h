/*
 * A more simple and easier to definition of an .obj file 
 * (comparted to ObjFile.h)
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

#include "../Math/Vector3.h"

#define FXS_OBJ_FILE2_MAX_STRLEN 16

enum {
	FXS_OBJ_FILE2_NORMALS = 1,
	FXS_OBJ_FILE2_TEXCOORDS = 2,
	FXS_OBJ_FILE2_MATERIALS = 4
};

typedef struct {
	int type; 			/* stores whether the face contains normals, 
						   texcoords or a material */
	int p0, p1, p2;
	int n0, n1, n2;
	int tc0, tc1, tc2;
	char name[FXS_OBJ_FILE2_MAX_STRLEN];
} FxsObjFile2Face;

typedef struct {
	int childId;
	int numChilds;
	int faceId;
	int numFaces;
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
