#include "ObjFile.h"
#include "../Dictionary/Dictionary.h"
#include "../Number/Number.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 1024

#define ERR_MSG(X) printf("In file: %s line: %d\n\tError reading line: %s\n", __FILE__, __LINE__, X);

/******************************************************************************
**  Line processing utils
******************************************************************************/

static int IsEndOfLine(char c)
{
	if (c == '\r' || c == '\n' || c == EOF || c == '\0') 
	{
	    return 1;
	}
	else
	{
		return 0;
	}
}

/*
** finds the first space of the line and returns the rest string from this 
** position (e.g. used to read in material names, object and group names,
** since they should be allowed to be not only one string). The line should 
** be trimmed before calling this function.
*/ 
static void GetRestOfLine(char* dst, const char* src)
{
	int i = 0;
	int j = 0;

	while (src[i] != ' ')
	{
		if (IsEndOfLine(src[i]))
		{
			return;
		}

		i++;
	}

	i++;

	while (!IsEndOfLine(src[i])) 
	{
		dst[j] = src[i];	

		i++;
		j++; 
	}

	dst[j] = '\0';
}

/*
** Trims the line from both ends.
*/ 
static void Trim(char* line)
{
	char tmp[MAX_STR_LEN];
	int i = 0, j = 0, k = 0;

	while(isspace(line[i]))
	{
		i++;
	}

	while (!IsEndOfLine(line[i])) 
	{
		tmp[j] = line[i];
	
		if (!isspace(tmp[j]))  /* save an index to the last non white space
							   ** character.
							   */
		{
		   	 k = j + 1;
		}

		j++;
		i++;
	}

	tmp[k] = '\0';

	strcpy(line, tmp);
}

/*
** replaces each white space char with the ' ' char. does not allow sequences
** two or more white spaces.
*/           
static void ProcessWhiteSpace(char* line)
{
	char tmp[MAX_STR_LEN];
	int i = 0, j = 0;
	int lcs = 0;

	/* convert all white spaces to ' ' */
	while (!IsEndOfLine(line[i]))
	{
		if (isspace(line[i])) 
		{
			line[i] = ' ';
		}

		i++;
	}

	/* remove sequences of white space */
	i = 0;
	while (!IsEndOfLine(line[i]))
	{
		if (!lcs || line[i] != ' ') 
		{
		    tmp[j] = line[i];
			j++;
		}

		if (line[i] == ' ') 
		{
		    lcs = 1;
		}
		else
		{
			lcs = 0;
		}		

		i++;	
	}

	tmp[j] = '\0';
	strcpy(line, tmp);
}

static void PreProcessLine(char* line)
{
	Trim(line);
	ProcessWhiteSpace(line);
}

/******************************************************************************
** 	Stuff for FxsObjGroup
******************************************************************************/

typedef struct FxsObjGroup_
{
	char* name;
	FxsListPtr faces;
}
FxsObjGroup;

const char* FxsObjGroupGetName(FxsObjGroupPtr group)
{
	if (!group) 
	{
	    return NULL;
	}

	return group->name;
}

FxsListPtr FxsObjGroupGetFaces(FxsObjGroupPtr group)
{
	if (!group) 
	{
	    return NULL;
	}

	return group->faces;
}

/*
** creates a group gives it a name and allocates a list for it.
*/ 
static FxsObjGroupPtr FxsObjGroupCreate(const char* name)
{
	size_t nameLen = 0;
 	FxsObjGroupPtr g = (FxsObjGroupPtr)malloc(sizeof(FxsObjGroup));
	
	if (!g) 
	{
	    return NULL;
	}

	memset(g, 0, sizeof(FxsObjGroup));

	if (name)
	{
		nameLen = strlen(name) + 1;
		g->name = (char*)malloc(nameLen);	

		if (!g->name) 
		{
			free(g);
	    	return NULL;
		}

		strcpy(g->name, name);
	}

	g->faces = FxsListCreate();

	if (!g->faces) 
	{
		if (g->name)
		{
		    free(g->name);
		}
		
		free(g);
		return NULL;
	}

	return g;
}

static void FxsObjGroupDestroy(FxsObjGroup* group)
{
	if (!group)
	{
	    return;
	}

	if (group->name) 
	{
	    free(group->name);
	}

	/* delete the list */
	FxsListDestroy(&group->faces);
	
	free(group);
}

static int FxsObjGroupAddFace(FxsObjGroupPtr group, FxsObjFace* face)
{
	if (!face || !group) 
	{
	    return 0;
	}

	if (!FxsListPushBack(group->faces, face))
	{
		return 0;
	}
	
	return 1;
}

/******************************************************************************
** 	Stuff for FxsObjObject
******************************************************************************/

typedef struct FxsObjObject_
{
	char* name;
	FxsListPtr groups;
}
FxsObjObject;

const char* FxsObjObjectGetName(FxsObjObjectPtr object)
{
	if (!object) 
	{
	    return NULL;
	}

	return object->name;
}

FxsListPtr FxsObjObjectGetGroups(FxsObjObjectPtr object)
{
	if (!object) 
	{
	    return NULL;
	}

	return object->groups;
}

static FxsObjObjectPtr FxsObjObjectCreate(const char* name)
{
	size_t nameLen = 0;
	FxsObjObjectPtr o = (FxsObjObjectPtr)malloc(sizeof(FxsObjObject));

	if (!o) 
	{
	    return NULL;
	}

	memset(o, 0, sizeof(FxsObjObject));

	if (name)
	{
		nameLen = strlen(name) + 1;
		o->name = (char*)malloc(nameLen);
	
		if (!o->name)
		{
	    	free(o);
			return NULL;
		}

		strcpy(o->name, name);
	}

	o->groups = FxsListCreate();

	if (!o->groups) 
	{
	    if (o->name)
		{
			free(o->name);
		}

		free(o);
		return NULL;
	}
	
	return o;
}

static void FxsObjObjectDestroy(FxsObjObjectPtr obj)
{
	FxsListIteratorPtr it = NULL;

	if (!obj)
	{
	  	return; 
	}

	if (obj->name) 
	{
	    free(obj->name);
	}

	/* delete the groups of the object */
	it = FxsListIteratorCreate(obj->groups, FXS_LIST_FRONT, FXS_LIST_FRONT_TO_BACK);

	if (it) 
	{
	    while (FxsListIteratorHasNext(it)) 
	    {
	        FxsObjGroupDestroy(FxsListIteratorNext(it));
	    }
	}

	FxsListIteratorDestroy(&it);	

	/* delete the list */
	FxsListDestroy(&obj->groups);

	/* delete the obj */
	free(obj);
}

/******************************************************************************
** 	Stuff for FxsObjFile
******************************************************************************/

typedef struct FxsObjFile_ 
{
	char* filename;
	char* mtlfilename; /* name of the assoc. .mtl file */

	FxsListPtr objects;	 /* objects contained in the .obj file */

	unsigned int numPositions;
	unsigned int numNormals;
	unsigned int numTexCoords;
	unsigned int numFaces;

	FxsVector3* positions;
	FxsObjFace* faces;
	FxsVector3* normals;
	FxsVector2* texCoords;

	unsigned int numMaterials;
	char** materialNames;

	/* memory pool information; the pool stores data for positions, normals
	** tex coordinates and faces of the .obj file. The pool is mainly used
	** to reduce fragementation. Prolly better allocation schemes exist, see 
	** e.g. http://www.gii.upv.es/tlsf/alloc/others
	*/
	char* pool; 	 /* pointer to the memory pool */
	size_t poolSize; /* size of the pool in bytes */
}
FxsObjFile;


static void CountGeometry(FxsObjFilePtr objFile, FILE* file)
{
	char line[MAX_STR_LEN];

	while (fgets(line, MAX_STR_LEN, file))
	{
		if (strstr(line, "v ")) 
		{
			objFile->numPositions++; 
		}
		else if (strstr(line, "vn ")) 
		{
			objFile->numNormals++; 
		}
		else if (strstr(line, "vt ")) 
		{
		    objFile->numTexCoords++;
		}
		else if (strstr(line, "f "))
		{
			objFile->numFaces++;
		}
	}
}



static int LoadFile(FxsObjFilePtr obj, FILE* file)
{
	FxsObjObjectPtr currentObject = NULL;
	FxsObjGroupPtr currentGroup = NULL;
	FxsVector3* currentPosition = NULL;
	FxsVector3* currentNormal = NULL;
	FxsVector2* currentTexCoord = NULL;
	FxsObjFace* currentFace = NULL;
	FxsDictionaryPtr matIds = NULL;
	unsigned int positionsLoaded = 0;
	unsigned int normalsLoaded = 0;
	unsigned int texCoordLoaded = 0;
	unsigned int facesLoaded = 0;
	char line[MAX_STR_LEN];
	char name[MAX_STR_LEN];
	int scres = 0;
	int i = 0;
	int currentMat = -1;
	size_t nameLen = 0;
	FxsNumberPtr matId = NULL;
	FxsListPtr matNameList = NULL;
	FxsListIteratorPtr matNameListIt = NULL;
	char* matName = NULL;

	/* create a dictionary for mananging the material (ids) */
	matIds = FxsDictionaryCreateWithTableSize(20);
	
	if (!matIds) 
	{
	    goto loadError;
	}

	/* create the objects list */
	obj->objects = FxsListCreate();

	if (!obj->objects) 
	{
	   	goto loadError;
	}

	/* create default object and group */
	currentObject = FxsObjObjectCreate(NULL);	

	if (!currentObject) 
	{
	    goto loadError;
	}

	currentGroup = FxsObjGroupCreate(NULL);

	if (!currentGroup) 
	{
	   	goto loadError;
	}

	/* add default obj and group to the file */
	if (!FxsListPushBack(obj->objects, currentObject))
	{
		goto loadError;
	}
	
	if (!FxsListPushBack(currentObject->groups, currentGroup))
	{
		goto loadError;
	}

	while (fgets(line, MAX_STR_LEN, file))
	{
		/**********************************************************************
		** load an object
		***********************************************************************/
		if (strstr(line, "o "))  
		{
			scres = sscanf(line, "o %s", name); 
			
			if (scres == 1) 
			{
				currentObject = FxsObjObjectCreate(name);	
			
				if (!currentObject) 
				{
				    goto loadError;
				}
			
				currentGroup = FxsObjGroupCreate(NULL);
			
				if (!currentGroup) 
				{
				   	goto loadError;
				}

				if (!FxsListPushBack(obj->objects, currentObject))
				{
					goto loadError;
				}
				
				if (!FxsListPushBack(currentObject->groups, currentGroup))
				{
					goto loadError;
				}

			}
			else
			{
				ERR_MSG(line);
				goto loadError;
			}
		} 	

		/**********************************************************************
		** load a group
		***********************************************************************/
		if (strstr(line, "g ")) 
		{
			scres = sscanf(line, "g %s", name);
			
			if (scres == 1) 
			{
	  			currentGroup = FxsObjGroupCreate(name);
			
				if (!currentGroup) 
				{
				   	goto loadError;
				}
				
				if (!FxsListPushBack(currentObject->groups, currentGroup))
				{
					goto loadError;
				}
			}
			else
			{
				ERR_MSG(line);
				goto loadError;
			}
		}

		/**********************************************************************
		** load a position
		***********************************************************************/
		if (strstr(line, "v ")) 
		{
			currentPosition = &(obj->positions[positionsLoaded]);
			scres = sscanf(
					line, 
					"v %f %f %f", 
					&currentPosition->x,
					&currentPosition->y,
					&currentPosition->z
				);

			if (scres == 3) 
			{
				positionsLoaded++;    
			}
			else
			{
				ERR_MSG(line);
				goto loadError;
			}
		}

		/**********************************************************************
		** load a normal
		***********************************************************************/
		if (strstr(line, "vn ")) 
		{
			currentNormal = &(obj->normals[normalsLoaded]);
			scres = sscanf(
					line, 
					"vn %f %f %f", 
					&currentNormal->x,
					&currentNormal->y,
					&currentNormal->z
				);

			if (scres == 3) 
			{
				normalsLoaded++;    
			}
			else
			{
				ERR_MSG(line);
				goto loadError;
			}
		}

		/**********************************************************************
		** load a tex coordinate
		***********************************************************************/
		if (strstr(line, "vt ")) 
		{
			currentTexCoord = &(obj->texCoords[texCoordLoaded]);
			scres = sscanf(
					line, 
					"vt %f %f", 
					&currentTexCoord->x,
					&currentTexCoord->y
				);

			if (scres == 2) 
			{
				texCoordLoaded++;    
			}
			else
			{
				ERR_MSG(line);
				goto loadError;
			}
		}

		/**********************************************************************
		** load a face
		***********************************************************************/
		if (strstr(line, "f ")) 
		{
			currentFace = &(obj->faces[facesLoaded]);
			memset(currentFace, 0, sizeof(FxsObjFace));
			currentFace->matIdx = currentMat;

			scres = sscanf(
					line, 
					"f %d/%d/%d %d/%d/%d %d/%d/%d",
					&currentFace->p0,
					&currentFace->tc0,
					&currentFace->n0,
					&currentFace->p1,
					&currentFace->tc1,
					&currentFace->n1,
					&currentFace->p2,
					&currentFace->tc2,
					&currentFace->n2
				);

			if (scres == 9) 
			{
			    facesLoaded++;
				FxsObjGroupAddFace(currentGroup, currentFace);
				continue;
			}

			scres = sscanf(
					line, 
					"f %d//%d %d//%d %d//%d",
					&currentFace->p0,
					&currentFace->n0,
					&currentFace->p1,
					&currentFace->n1,
					&currentFace->p2,
					&currentFace->n2
				);

			if (scres == 6) 
			{
			    facesLoaded++;
				FxsObjGroupAddFace(currentGroup, currentFace);
				continue;
			}

			scres = sscanf(
					line, 
					"f %d/%d %d/%d %d/%d",
					&currentFace->p0,
					&currentFace->tc0,
					&currentFace->p1,
					&currentFace->tc1,
					&currentFace->p2,
					&currentFace->tc2
				);

			if (scres == 6) 
			{
			    facesLoaded++;
				FxsObjGroupAddFace(currentGroup, currentFace);
				continue;
			}
			
			scres = sscanf(
					line, 
					"f %d/%d/ %d/%d/ %d/%d/",
					&currentFace->p0,
					&currentFace->tc0,
					&currentFace->p1,
					&currentFace->tc1,
					&currentFace->p2,
					&currentFace->tc2
				);

			if (scres == 6) 
			{
			    facesLoaded++;
				FxsObjGroupAddFace(currentGroup, currentFace);
				continue;
			}

			scres = sscanf(
					line, 
					"f %d %d %d",
					&currentFace->p0,
					&currentFace->p1,
					&currentFace->p2
				);

			if (scres == 3) 
			{
			    facesLoaded++;
				FxsObjGroupAddFace(currentGroup, currentFace);
				continue;
			}

			ERR_MSG(line);
			goto loadError;
		}


		/**********************************************************************
		** load name of the mtl file
		***********************************************************************/
		if (strstr(line, "mtllib ")) 
		{
		   	scres = sscanf(line, "mtllib %s", name); 

			if (scres == 1) 
			{
				nameLen = strlen(name) + 1;
			    obj->mtlfilename = (char*)malloc(nameLen);
				
				if (!obj->mtlfilename)
				{
				    goto loadError;
				}

				strcpy(obj->mtlfilename, name);
			}
			else
			{
				ERR_MSG(line);
				goto loadError;
			}
		}

		/**********************************************************************
		** load name for the material and associate it with an index 
		***********************************************************************/
		if (strstr(line, "usemtl "))
		{
			scres = sscanf(line, "usemtl %s", name);
		
			if (scres == 1) 
			{
				if (FxsDictionaryContains(matIds, name))
				{
					matId = FxsDictionaryFind(matIds, name);	
					currentMat = FxsNumberAsInt(matId);
				}
				else
				{
					currentMat = (int)FxsDictionaryGetSize(matIds);
					matId = FxsNumberCreateWithInt(currentMat);

					if (!matId) 
					{
						ERR_MSG(line);
					    goto loadError;
					}

					FxsDictionaryInsert(
						matIds, 
						name, 
						matId
					);	
				}
			}
			else
			{
				ERR_MSG(line);
                goto loadError;
			}
		}		

	
	}	/* of while */

	/* update the face indices */
	for (i = 0; i < obj->numFaces; i++) 
	{
	   	obj->faces[i].p0--; 
	   	obj->faces[i].tc0--; 
	   	obj->faces[i].n0--; 
	   	obj->faces[i].p1--; 
	   	obj->faces[i].tc1--; 
	   	obj->faces[i].n1--; 
	   	obj->faces[i].p2--; 
	   	obj->faces[i].tc2--; 
	   	obj->faces[i].n2--; 
	}

	/* save the names of the materials */
	obj->numMaterials = (unsigned int)FxsDictionaryGetSize(matIds);

    if (obj->numMaterials)
    {
        matNameList = FxsDictionaryGetKeys(matIds);

        matNameListIt = FxsListIteratorCreate(
                matNameList,
                FXS_LIST_FRONT,
                FXS_LIST_FRONT_TO_BACK
            );


        obj->materialNames = (char**)malloc(sizeof(char**)*obj->numMaterials);

        if (!obj->materialNames)
        {
            goto loadError;
        }

        memset(obj->materialNames, 0, sizeof(char**)*obj->numMaterials);

        while (FxsListIteratorHasNext(matNameListIt))
        {
            matName = FxsListIteratorNext(matNameListIt);
            nameLen = strlen(matName) + 1;
            matId = FxsDictionaryFind(matIds, matName);
            obj->materialNames[FxsNumberAsInt(matId)] = (char*)malloc(nameLen);
            strcpy(obj->materialNames[FxsNumberAsInt(matId)], matName);
            FxsNumberDestroy(&matId);
        }

        FxsListIteratorDestroy(&matNameListIt);
    }


	FxsDictionaryDestroy(&matIds);

	return 1;

loadError:

    matNameList = FxsDictionaryGetKeys(matIds);
    matNameListIt = FxsListIteratorCreate(
            matNameList,
            FXS_LIST_FRONT,
            FXS_LIST_FRONT_TO_BACK
        );

    while (FxsListIteratorHasNext(matNameListIt))
    {
        matName = FxsListIteratorNext(matNameListIt);
        nameLen = strlen(matName) + 1;
        matId = FxsDictionaryFind(matIds, matName);
        FxsNumberDestroy(&matId);
    }

	FxsDictionaryDestroy(&matIds);

    return 0;
}

FxsObjFilePtr FxsObjFileCreateWithFile(const char* filename)
{
	FILE* file = NULL;
	FxsObjFilePtr objFile = NULL;
//	int succ = 1;     				/* assume we are succesful */
	size_t strLen = 0;
	size_t memOffset = 0;  			/* memory offset into the pool */

	if (!filename) 
	{
		return NULL;
	}

	file = fopen(filename, "r");

	if (!file) 
	{
	    return NULL;
	}

	objFile = (FxsObjFilePtr)malloc(sizeof(FxsObjFile));
	
	if (!objFile) 
	{
		goto error;	
	}
	
	memset(objFile, 0, sizeof(FxsObjFile));

	/* copy the name of the file */
	strLen = strlen(filename) + 1;
	objFile->filename = (char*)malloc(strLen);
	
	if (!objFile->filename) 
	{
		goto error;
	}

	strcpy(objFile->filename, filename);
	
	/* count # positions, normals, texcoords, faces */
	CountGeometry(objFile, file);

	/* allocate all (most) of the memory in one block to minimize fragmentation
	*/
	objFile->poolSize = (objFile->numPositions + objFile->numNormals)*sizeof(FxsVector3) + 
		objFile->numTexCoords*sizeof(FxsVector2) + 
		objFile->numFaces*sizeof(FxsObjFace);

	objFile->pool = (char*)malloc(objFile->poolSize);

	if (!objFile->pool)
	{
		goto error;
	}

	if (!objFile->numPositions || !objFile->numFaces) 
	{
	   	goto error; 
	}	

	objFile->positions = (FxsVector3*)objFile->pool;
	memOffset += sizeof(FxsVector3)*objFile->numPositions;
	objFile->faces = (FxsObjFace*)(objFile->pool + memOffset);
	memOffset += sizeof(FxsObjFace)*objFile->numFaces;

	if (!objFile->numNormals) 
	{
		objFile->normals = NULL;
	}
	else
	{
		objFile->normals = (FxsVector3*)(objFile->pool + memOffset);
	}

	memOffset += sizeof(FxsVector3)*objFile->numNormals;

	if (!objFile->numTexCoords) 
	{
	    objFile->texCoords = NULL;
	}
	else
	{
		objFile->texCoords = (FxsVector2*)(objFile->pool + memOffset);
	}

	/* load the file */
	rewind(file);
	if (!LoadFile(objFile, file))
	{
		goto error;
	}

	/* clean up */
	fclose(file);
	return objFile;

error:
	FxsObjFileDestroy(&objFile);
	fclose(file);
	return NULL;
}

void FxsObjFileDestroy(FxsObjFilePtr* objFile)
{
	int i = 0;
	FxsListIteratorPtr li = NULL;

	if (!objFile || !(*objFile)) 
	{
	   	return; 
	}

	/* release the memory pool */
	if ((*objFile)->pool) 
	{
	    free((*objFile)->pool);
	}

	/* release objects */
	li = FxsListIteratorCreate(
			(*objFile)->objects, 
			FXS_LIST_FRONT, 
			FXS_LIST_FRONT_TO_BACK
		);

	if (!li) 
	{
	   	printf("Could not release .obj file\n");
		return;
	}

	while (FxsListIteratorHasNext(li))
	{
		FxsObjObjectDestroy(FxsListIteratorNext(li));
	}

	FxsListIteratorDestroy(&li);
	FxsListDestroy(&(*objFile)->objects);

    /* release material names */
	if ((*objFile)->materialNames) 
	{
		for (i = 0; i < (*objFile)->numMaterials; i++) 
	    {
	    	if ((*objFile)->materialNames[i]) 
	       	{
	       		free((*objFile)->materialNames[i]); 	 
	       	}
	    }
		
		free((*objFile)->materialNames);
	}

	/* release .mtl filename */
	if ((*objFile)->mtlfilename) 
	{
	    free((*objFile)->mtlfilename);
	}

	/* release the filename */
	if ((*objFile)->filename) 
	{
	    free((*objFile)->filename);
	}

	/* free the actual object file */
	free(*objFile);
	objFile = NULL;
}

int FxsObjFileGetPosition(FxsObjFilePtr file, FxsVector3* pos, unsigned int id)
{
	if (!file || file->numPositions <= id) 
	{
	    return 0;
	}

	*pos = file->positions[id];

	return 1;
}

int FxsObjFileGetNormal(FxsObjFilePtr file, FxsVector3* normal, unsigned int id)
{
	if (!file || file->numNormals <= id) 
	{
	    return 0;
	}

	*normal = file->normals[id];

	return 1;
}

int FxsObjFileGetTexCoord(FxsObjFilePtr file, FxsVector2* tc, unsigned int id)
{
	if (!file || file->numTexCoords <= id) 
	{
	   	return 0; 
	}

	*tc = file->texCoords[id];

	return 1;
}

const char* FxsObjFileGetMaterialName(FxsObjFilePtr file, unsigned int id)
{
	if (!file || file->numMaterials <= id) 
	{
	    return NULL;
	}

	return file->materialNames[id];
}

const char* FxsObjFileGetMtlName(FxsObjFilePtr file)
{
	return file->mtlfilename;
}

unsigned int FxsObjFileGetNumPositions(FxsObjFilePtr file)
{
	if (!file) 
	{
	    return 0; 
	}

	return file->numPositions; 
}

unsigned int FxsObjFileGetNumTexCoords(FxsObjFilePtr file)
{
	if (!file) 
	{
	    return 0; 
	}

	return file->numTexCoords; 
}

unsigned int FxsObjFileGetNumNormals(FxsObjFilePtr file)
{
	if (!file) 
	{
	    return 0; 
	}

	return file->numNormals; 
}

FxsListPtr FxsObjFileGetObjects(FxsObjFilePtr file)
{
    if (!file)
    {
        return 0;
    }

	return file->objects;
}

unsigned int FxsObjFileGetNumMaterials(FxsObjFilePtr file)
{
    if (!file)
    {
        return 0;
    }

    return file->numMaterials;
}
