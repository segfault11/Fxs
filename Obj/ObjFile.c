#include "ObjFile.h"
#include <stdio.h>
#include <string.h>

#define MAX_STR_LEN 1024

#define ERR_MSG(X) printf("In file: %s line: %d\n\tError reading line: %s\n", __FILE__, __LINE__, X);


/******************************************************************************
** 	Stuff for FxsObjGroup
******************************************************************************/

typedef struct FxsObjGroup_
{
	char* name;
	FxsListPtr faces;
}
FxsObjGroup;

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
	FxsListIteratorPtr it = NULL;

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

FxsObjObjectPtr FxsObjObjectCreate(const char* name)
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
	
		if (o->name) 
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

void FxsObjObjectDestroy(FxsObjObjectPtr obj)
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

int FxsObjObjectAddGroup(FxsObjObjectPtr obj, FxsObjGroupPtr group)
{
	if (!obj) 
	{
	    return 0;
	}

	FxsListPushBack(obj->groups, group);
	return 1;
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
	unsigned int positionsLoaded = 0;
	unsigned int normalsLoaded = 0;
	unsigned int texCoordLoaded = 0;
	unsigned int facesLoaded = 0;
	char line[MAX_STR_LEN];
	char name[MAX_STR_LEN];
	int scres = 0;

	/* create the objects list */
	obj->objects = FxsListCreate();

	if (!obj->objects) 
	{
	   	return 0; 
	}

	/* create default object and group */
	currentObject = FxsObjObjectCreate(NULL);	

	if (!currentObject) 
	{
	    return 0;
	}

	currentGroup = FxsObjGroupCreate(NULL);

	if (!currentGroup) 
	{
	   	return 0; 
	}

	/* add default obj and group to the file */
	if (FxsListPushBack(obj->objects, currentObject))
	{
		return 0;
	}
	
	if (FxsListPushBack(currentObject->groups, currentGroup))
	{
		return 0;
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
				    return 0;
				}
			
				currentGroup = FxsObjGroupCreate(NULL);
			
				if (!currentGroup) 
				{
				   	return 0; 
				}

				if (FxsListPushBack(obj->objects, currentObject))
				{
					return 0;
				}
				
				if (FxsListPushBack(currentObject->groups, currentGroup))
				{
					return 0;
				}

			}
			else
			{
				ERR_MSG(line);
				return 0;
			}
		} 	

		/**********************************************************************
		** load a group
		***********************************************************************/
		if (strstr(line, "g ")) 
		{
			scres = sscanf(line, "o %s", name);
			
			if (scres == 1) 
			{
	  			currentGroup = FxsObjGroupCreate(name);
			
				if (!currentGroup) 
				{
				   	return 0; 
				}
				
				if (FxsListPushBack(currentObject->groups, currentGroup))
				{
					return 0;
				}
			}
			else
			{
				ERR_MSG(line);
				return 0;
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
				return 0;
			}

		}
	
	}	

	return 1;
}

FxsObjFilePtr FxsObjFileCreate(const char* filename)
{
	FILE* file = NULL;
	FxsObjFilePtr objFile = NULL;
	int succ = 1;     				/* assume we are succesful */
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


