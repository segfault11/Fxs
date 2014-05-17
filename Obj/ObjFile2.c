#include "ObjFile2.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../Array/Array.h"

/***** Node Definition ********************************************************/
typedef struct Node Node;

struct Node {
	int faceId;
	int numFaces;
	Node *next; 	/* siblings to the node */   	 
	Node *child; 	/* children */
};


static Node* NodeCreate()
{
	Node *n = calloc(1, sizeof(Node));

	if (!n) {
	    return NULL;
	}

	n->faceId = -1;
	
	return n;
}

/*
** Destroys the node and all its children.
*/ 
static void NodeDestroy(Node *node)
{
	Node *c = NULL, *c2;

	if (!node) {
	    return;
	}

	c = node->child;

	while (c) {
		c2 = c->next;
		NodeDestroy(c);
		c = c2;
	}

	free(node);
}

/*
** Adds a child at the beginning of the child list of "parent"
*/ 
static void NodePushChild(Node *parent, Node *child)
{
	assert(child && parent);

	child->next = parent->child;
	parent->child = child;
}

/***** File Definition ********************************************************/
typedef struct {
	Node *root;

	FxsArray *faces;
	FxsArray *positions;
	FxsArray *normals;
	FxsArray *texCoords;
} File;

static File* FileCreate()
{
	File *f = malloc(sizeof(File));

	if (!f) {
	    return NULL;
	}

	f->root = NodeCreate();
	f->faces = FxsArrayCreate(1, sizeof(FxsObjFile2Face));
	f->positions = FxsArrayCreate(1, sizeof(FxsVector3));
	f->normals = FxsArrayCreate(1, sizeof(FxsVector3));
	f->texCoords = FxsArrayCreate(1, sizeof(FxsVector2));
	
	/* TODO: check if create succeeded */

	return f;
}

static void FileDestroy(File *file)
{
	assert(file);

	NodeDestroy(file->root);
	FxsArrayDestroy(file->faces);
	FxsArrayDestroy(file->positions);
	FxsArrayDestroy(file->normals);
	FxsArrayDestroy(file->texCoords);

	free(file);
}

static void FileUpdateWithLine(File *file, const char *line)
{
	if (strstr(line, "v ") == line) {
	    
	} else if (strstr(line, "vn") == line) {

	} else if (strstr(line, "vt") == line) {

	} else if (strstr(line, "f ") == line) {

	} else if (strstr(line, "g ") == line) {
	
	} else if (strstr(line, "o ") == line) {

	} 
}




FxsObjFile2* FxsObjFile2Create(const char *filename)
{
	FxsObjFile2 *obj = NULL;
	FILE *f = NULL;

	f = fopen(filename, "r");

	if (!f) {
	    return NULL;
	}

	obj = calloc(1, sizeof(FxsObjFile2));

	if (!obj) {
	    fclose(f);
	}

	return obj;

error:
	fclose(f);
	FxsObjFile2Destroy(&obj);
	return NULL;
}

void FxsObjFile2Destroy(FxsObjFile2 **file)
{

}
