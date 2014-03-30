#include "ObjFile.h"
#include <string.h>

typedef struct FxsObjFile_ 
{
	const char* filename;
	FxsListPtr objects;	
}
FxsObjFile;


/******************************************************************************
** 	Stuff for FxsObjFace
******************************************************************************/

FxsObjFace* FxsObjFaceCreateWithFace(const FxsObjFace* f)
{
	FxsObjFace* of = (FxsObjFace*)malloc(sizeof(FxsObjFace));

	if (!of) 
	{
	    return NULL;
	}

	if (!f) 
	{
		free(of);
	    return NULL;
	}
	
	memcpy(of, f, sizeof(FxsObjFace));

	return of;
}

void FxsObjFaceDestroy(FxsObjFace** face)
{
	if (!face || !(*face)) 
	{
		return;
	}

	free(*face);
	*face = NULL;
}



/******************************************************************************
** 	Stuff for FxsObjGroup
******************************************************************************/

typedef struct FxsObjGroup_
{
	const char* name;
	FxsListPtr faces;
}
FxsObjGroup;

/*
** creates a group gives it a name and allocates a list for it.
*/ 
static FxsObjGroupPtr FxsObjGroupCreate(const char* name)
{
	size_t nameLen = strlen(name);
	FxsObjGroupPtr g = (FxsObjGroupPtr)malloc(sizeof(FxsObjGroup));
	
	if (!g) 
	{
	    return NULL;
	}

	g->name = (char*)malloc(nameLen);	

	if (!g->name) 
	{
		free(g);
	    return NULL;
	}

	strcpy(g->name, name);

	g->faces = FxsListCreate();

	if (!g->faces) 
	{
	    free(g->name);
		free(g);
		return NULL;
	}

	return g;
}

static void FxsObjGroupDestroy(FxsObjGroup** group)
{
	FxsListIteratorPtr it = NULL;

	if (!group || !(*group)) 
	{
	    return;
	}

	if ((*group)->name) 
	{
	    free((*group)->name);
	}

	/* delete all faces in the group */
	it = FxsListIteratorCreate((*group)->faces, FXS_LIST_BEGIN);

	if (it)
	{
		while (FxsListIteratorHasNext(it))
		{
			FxsObjFaceDestroy(&FxsListIteratorNext());
		}
	}	

	FxsListIteratorDestroy(&it);

	/* delete the list */
	FxsListDestroy(&(*group)->faces);
	
	free(*group);
	*group = NULL;
}

static int FxsObjGroupAddFace(FxsObjGroupPtr group, const FxsObjFace* face)
{
	FxsObjFace* f = NULL;

	if (!face) 
	{
	    return 0;
	}

	f = FxsObjFaceCreateWithFace(face);
	
	if (!f) 
	{
	    return 0;
	}

	if (!FxsListPushBack(group->faces, f))
	{
		FxsObjFaceDestroy(&f);
		return 0;
	}
	
	return 1;
}

/******************************************************************************
** 	Stuff for FxsObjObject
******************************************************************************/

typedef struct FxsObjObject_
{
	const char* name;
	FxsListPtr groups;
}
FxsObjObject;

FxsObjObjectPtr FxsObjObjectCreate(const char* name)
{
	size_t nameLen = strlen(name) + 1;
	FxsObjObjectPtr o = (FxsObjObjectPtr)malloc(sizeof(FxsObjObject));

	if (!o) 
	{
	    return NULL;
	}

	o->name = (char*)malloc(nameLen);
	
	if (o->name) 
	{
	    free(o);
		return NULL;
	}

	strcpy(o->name, name);

	o->groups = FxsListCreate();

	if (!o->groups) 
	{
	    free(o->name);
		free(o);
		return NULL;
	}
	
	return o;
}

void FxsObjObjectDestroy(FxsObjObjectPtr* obj)
{
	FxsListIteratorPtr it = NULL;

	if (!obj || !(*obj)) 
	{
	  	return; 
	}

	if ((*obj)->name) 
	{
	    free((*obj)->name);
	}

	/* delete the groups of the object */
	it = FxsListIteratorCreate((*obj)->groups);

	if (it) 
	{
	    while (FxsListIteratorHasNext(it)) 
	    {
	        FxsObjGroupDestroy(FxsListIteratorNext(it));
	    }
	}

	FxsListIteratorDestroy(&it);	

	/* delete the list */
	FxsListDestroy(&(*obj)->groups);

	/* delete the obj */
	free(*obj);
	*obj = NULL;
}

int FxsObjObjectAddGroup(FxsObjObjectPtr obj, FxsObjGroupPtr group)
{
	if (!obj) 
	{
	    return 0;
	}

	FxsListPushBack(obj->groups, group);
}

/******************************************************************************
** 	Stuff for FxsObjFile
******************************************************************************/
