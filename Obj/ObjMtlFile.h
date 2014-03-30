/*
 * Mtl file parser
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
#ifndef OBJMTLFILE_H
#define OBJMTLFILE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct FxsObjMtlFile_* FxsObjMtlFilePtr;
typedef struct FxsObjMaterial_* FxsObjMaterialPtr;

/*
** Creates the mtl file from a file referenced by filename.
**
** @param mtlfile The pointer to the mtl file handle to be created.
** @param filename Filename of the mtl file.
*/ 
int FxsObjMtlFileCreateWithFile(
	FxsObjMtlFilePtr* mtlfile, 
	const char* filename
);

/*
** Releases the material file and all its materials.
**
** @param mtlfile The mtl file to be released.
*/ 
void FxsObjMtlFileDestroy(FxsObjMtlFilePtr* mtlfile);

/*
** Gets the matrial from the mtl file that corresponds to "name". The material
** is managed by the mtl file do not free it or dont free the mtl file before
** using the material.
**
** @param name Name of the material.
** @param mtlfile Mtl file that contains the material.
**
** @param NULL if the material was not found, otherwise the material.
*/ 
FxsObjMaterialPtr FxsObjMtlFileGetMaterialWithName(
	FxsObjMtlFilePtr mtlfile,
	const char* name
);

float FxsObjMaterialGetComponentWithAttributeAsFloat(
	FxsObjMaterialPtr material,
	const char* name, 
	unsigned int idx
);

unsigned int FxsObjMaterialGetAttributeSize(
	FxsObjMaterialPtr material,
	const char* name
);
	
#ifdef __cplusplus
}
#endif

#endif /* end of include guard: OBJMTLFILE_H */
