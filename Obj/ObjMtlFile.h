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
** Gets an element of the material attribute array. The material attribute
** is identified by its name the element is identified by its index. E.g.
** if the material attribute was defined by the following line in the mtl file
**
** 		Ka 0.640000 0.042222 0.034073
**
** to get the the 3rd element of the attribute "Ka", one would have to call
**
** 		FxsObjMaterialGetAttributeElementAsString(nat, "Ka", 2);
**
** @param material The material we want the attribute information from.
** @param name  The name if the attribute.
** @param idx The index for the element in the attribute array,
** @return The element in form of a string, or NULL if something goes wrong.
*/ 
const char* FxsObjMaterialGetAttributeElementAsString(
	FxsObjMaterialPtr material,
	const char* name, 
	unsigned int idx
);

/*
** Gets an element of the material attribute array. The material attribute
** is identified by its name the element is identified by its index. E.g.
** if the material attribute was defined by the following line in the mtl file
**
** 		Ka 0.640000 0.042222 0.034073
**
** to get the the 3rd element of the attribute "Ka", one would have to call
**
** 		FxsObjMaterialGetAttributeElementAsString(nat, "Ka", 2);
**
** @param material The material we want the attribute information from.
** @param name  The name if the attribute.
** @param idx The index for the element in the attribute array,
** @return The element in form of a float, or FLT_MAX if something goes wrong.
*/
float FxsObjMaterialGetAttributeElementAsFloat(
	FxsObjMaterialPtr material,
	const char* name, 
	unsigned int idx
);

/*
** Gets the number of elements for the attribute of a material.
**
** @param material The material that contains the element.
** @param name The name of the attribute would would like to know the # of 
**        elements
** @return The number of elements for an material attribute.
*/ 
unsigned int FxsObjMaterialGetAttributeSize(
	FxsObjMaterialPtr material,
	const char* name
);

/*
** Creates the mtl file from a file referenced by filename.
**
** @param mtlfile The pointer to the mtl file handle to be created.
** @param filename Filename of the mtl file.
*/ 
FxsObjMtlFilePtr FxsObjMtlFileCreateWithFile(const char* filename);

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


	
#ifdef __cplusplus
}
#endif

#endif /* end of include guard: OBJMTLFILE_H */
