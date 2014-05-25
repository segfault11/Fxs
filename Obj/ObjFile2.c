#include "ObjFile2.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../Array/Array.h"

#define MAX_STRLEN 256

#define FREE(X) if(X) free(X);

/***** Node Definition ********************************************************/
typedef struct Node Node;

/*
** Nodes make up the scene graph of the .obj file.
*/ 
struct Node {
	char name[FXS_OBJ_FILE2_MAX_STRLEN];
	char materialName[FXS_OBJ_FILE2_MAX_STRLEN];
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

/*
** Counts # of nodes in a tree
*/ 
static size_t NodeCount(Node *node)
{
	Node *c;
	size_t n = 0; /* # of nodes in the sub-tree */

	if (!node) {
	    return 0;
	}

	c = node->child;

	while (c) {
		n += NodeCount(c);
		c = c->next;
	}
	
	return 1 + n;
}

/*
** Counts the total number of faces in a tree
*/ 
static size_t NodeCountFaces(Node *node)
{
	size_t n = 0;
	Node *c = NULL;

	if (!node) {
	    return 0;
	}
    
    c = node->child;

	while (c) {
		n += NodeCountFaces(c);
	    c = c->next;
	}

	return node->numFaces + n;
}

/*
** Deletes sub trees that do not contain faces.
*/ 
static void NodeTrim(Node *node) 
{
	Node *c, *cnext, *cprev;

	if (!node) {
	    return;
	}

	c = node->child;
	cprev = node->child;

	/* delete subtrees if they do not contain faces */
	while (c) {
		cnext = c->next;

	    if (!NodeCountFaces(c)) {
			if (c == node->child) {
			    node->child = cnext;
			} else {
				cprev->next = cnext;
			}

			NodeDestroy(c);			
		} else {
			cprev = c;
		}
	
		c = cnext;
	}

	/* trim subtrees */
	c = node->child;

	while (c) {
	    NodeTrim(c);
		c = c->next;
	}
}

/***** File Definition ********************************************************/

/*  
** Temporary representation of an .obj file. Stores the scene graph as a linked
** structure (not linear) in memory. Lateron this struct is mapped to the
** FxsObjFile2 structure.
*/
typedef struct {
	Node *root;
	Node *current; 				/* the latest created node */

	FxsArray *faces;
	FxsArray *positions;
	FxsArray *normals;
	FxsArray *texCoords;

	char mtlFilename[FXS_OBJ_FILE2_MAX_STRLEN];
} File;

static File* FileCreate()
{
	File *f = calloc(1 ,sizeof(File));
	Node *o, *g; 					/* default object and group nodes */

	if (!f) {
	    return NULL;
	}

	f->positions = FxsArrayCreate(1, sizeof(FxsVector3));
	f->normals = FxsArrayCreate(1, sizeof(FxsVector3));
	f->texCoords = FxsArrayCreate(1, sizeof(FxsVector2));
	f->faces = FxsArrayCreate(1, sizeof(FxsObjFile2Face));
	f->root = NodeCreate();
	o = NodeCreate();
	g = NodeCreate();

	/* TODO: check if create succeeded */

	/* initially the graph has a default object and group node */
	NodePushChild(o, g);
	NodePushChild(f->root, o);
	f->current = g;	
	return f;
}

static void FileDestroy(File *file)
{
	assert(file);

	NodeDestroy(file->root);
	FxsArrayDestroy(&file->faces);
	FxsArrayDestroy(&file->positions);
	FxsArrayDestroy(&file->normals);
	FxsArrayDestroy(&file->texCoords);

	free(file);
}

static void LoadPosition(File *file, const char *line)
{
	FxsVector3 v;

	if (3 == sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z)) {
	    FxsArrayPush(file->positions, &v);
	}
}

static void LoadNormal(File *file, const char *line)
{
	FxsVector3 v;

	if (3 == sscanf(line, "vn %f %f %f", &v.x, &v.y, &v.z)) {
	    FxsArrayPush(file->normals, &v);
	}
}

static void LoadTexCoord(File *file, const char *line)
{
	FxsVector2 v;

	if (3 == sscanf(line, "vt %f %f", &v.x, &v.y)) {
	    FxsArrayPush(file->texCoords, &v);
	}
}

static void LoadFace(File *file, const char *line)
{
	FxsObjFile2Face f;
	int found = 0;
    
    memset(&f, 0, sizeof(FxsObjFile2Face));

	if (3 == sscanf(line, "f %d %d %d", &f.p0, &f.p1, &f.p2)) {
	    found = 1;
	} else if (6 == sscanf(line, "f %d//%d %d//%d %d//%d", 
		&f.p0, &f.n0, &f.p1, &f.n1, &f.p2, &f.n2)) {
		f.type |= FXS_OBJ2_FACE_NORMALS;
	    found = 1;
	} else if (6 == sscanf(line, "f %d/%d %d/%d %d/%d", 
		&f.p0, &f.tc0, &f.p1, &f.tc1, &f.p2, &f.tc2)) {
		f.type |= FXS_OBJ2_FACE_TEXCOORDS;
	    found = 1;
	} else if (6 == sscanf(line, "f %d/%d/ %d/%d/ %d/%d/", 
		&f.p0, &f.tc0, &f.p1, &f.tc1, &f.p2, &f.tc2)) {
		f.type |= FXS_OBJ2_FACE_TEXCOORDS;
	    found = 1;
	} else if (6 == sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
		&f.p0, &f.tc0, &f.n0, &f.p1, &f.tc1, &f.n1, &f.p2, &f.tc2, &f.n2)) {
		f.type |= FXS_OBJ2_FACE_NORMALS;
		f.type |= FXS_OBJ2_FACE_TEXCOORDS;
	    found = 1;
	}

	if (!found) {
	    return;
	}
    
    f.p0--;
    f.p1--;
    f.p2--;

    f.n0--;
    f.n1--;
    f.n2--;

    f.tc0--;
    f.tc1--;
    f.tc2--;

	FxsArrayPush(file->faces, &f);
	file->current->numFaces++;
}

static void LoadGroup(File *file, const char *line)
{
	Node *n = NodeCreate();
	char name[FXS_OBJ_FILE2_MAX_STRLEN];

	if (1 != sscanf(line, "g %s", name)) {
	    NodeDestroy(n);
		return;
	}

	strncpy(n->name, name, FXS_OBJ_FILE2_MAX_STRLEN);	
	n->faceId = (int)file->faces->count;
	NodePushChild(file->root->child, n);
	file->current = n;
}

static void LoadObject(File *file, const char *line)
{
	Node *o = NodeCreate();
	Node *g = NodeCreate();
	char name[FXS_OBJ_FILE2_MAX_STRLEN];

	if (1 != sscanf(line, "o %s", name)) {
	    NodeDestroy(o);
	    NodeDestroy(g);
		return;
	}

	strncpy(o->name, name, FXS_OBJ_FILE2_MAX_STRLEN);	
	g->faceId = (int)file->faces->count;
	NodePushChild(file->root, o);
	NodePushChild(file->root->child, g);
	file->current = g;
}

static void LoadMaterialGroup(File *file, const char *line)
{
	Node *n = NodeCreate();
	char name[FXS_OBJ_FILE2_MAX_STRLEN];
	
	if (1 != sscanf(line, "usemtl %s", name)) {
		NodeDestroy(n);
		return;
	}

	strncpy(n->materialName, name, FXS_OBJ_FILE2_MAX_STRLEN);
	n->faceId = (int)file->faces->count;
	NodePushChild(file->root->child->child, n);
	file->current = n;
}

static void LoadMaterialFileName(File *file, const char *line)
{
	char name[FXS_OBJ_FILE2_MAX_STRLEN];

	if (1 == sscanf(line, "mtllib %s", name)) {
		strncpy(file->mtlFilename, name, FXS_OBJ_FILE2_MAX_STRLEN);
	}
}

static void FileUpdateWithLine(File *file, const char *line)
{
	if (strstr(line, "v ") == line) {
	    LoadPosition(file, line);
	} else if (strstr(line, "vn") == line) {
		LoadNormal(file, line);
	} else if (strstr(line, "vt") == line) {
		LoadTexCoord(file, line);
	} else if (strstr(line, "f ") == line) {
		LoadFace(file, line);
	} else if (strstr(line, "g ") == line) {
		LoadGroup(file, line);	
	} else if (strstr(line, "o ") == line) {
		LoadObject(file, line);
	} else if (strstr(line, "usemtl ") == line) {
		LoadMaterialGroup(file, line);
	} else if (strstr(line, "mtllib ") == line) {
		LoadMaterialFileName(file, line);
	} 
}

/***** File to FxsObjFile2 conversion *****************************************
**
** 	Method(s) that convert the "File" structure to the final FxsObjFile2 struct
**
******************************************************************************/

static void FxsObjFile2InitHeader(FxsObjFile2 *obj, const File *file)
{
	obj->numPostitions = (int)file->positions->count;
	obj->numNormals = (int)file->normals->count;
	obj->numTexCoords = (int)file->texCoords->count;
	obj->numFaces = (int)file->faces->count;
	obj->numNodes = (int)NodeCount(file->root);
	strncpy(obj->mtlFilename, file->mtlFilename, FXS_OBJ_FILE2_MAX_STRLEN);
}

static void FxsObjFile2InitGeometry(FxsObjFile2 *obj, const File *file)
{
	/* TODO: check if malloc fails */
	size_t s;

	s = file->positions->count * sizeof(FxsVector3);
	obj->positions = malloc(s);
	memcpy(obj->positions, file->positions->data, s);
	
	s = file->normals->count * sizeof(FxsVector3);
	obj->normals = malloc(s);
	memcpy(obj->normals, file->normals->data, s);

	s = file->texCoords->count * sizeof(FxsVector2);
	obj->texCoords = malloc(s);
	memcpy(obj->texCoords, file->texCoords->data, s);

	s = file->faces->count * sizeof(FxsObjFile2Face);
	obj->faces = malloc(s);
	memcpy(obj->faces, file->faces->data, s);
}

static void MapNodeToFxsObjFile2Node(FxsObjFile2Node *objNodes, int i, Node **nodes, int *j)
{
	size_t n = 0;
	Node *c = nodes[i]->child;

	objNodes[i].faceId = nodes[i]->faceId;
	objNodes[i].numFaces = nodes[i]->numFaces;

	if (objNodes[i].numFaces == 0) {
	    objNodes[i].faceId = -1;
	}

	objNodes[i].childId = *j;

	strncpy(objNodes[i].name, nodes[i]->name, FXS_OBJ_FILE2_MAX_STRLEN);
	strncpy(objNodes[i].materialName, nodes[i]->materialName, FXS_OBJ_FILE2_MAX_STRLEN);

	while (c) {
		n++;
		nodes[*j] = c;
		(*j)++;
	    c = c->next;
	}

	objNodes[i].numChilds = (unsigned int)n;

	if (!n) {
	    objNodes[i].childId = -1;
	}
}

static void FxsObjFile2InitSceneGraph(FxsObjFile2 *obj, File *file)
{
	/* TODO: check if malloc fails */
	int i = 0, j = 1;
	Node **nodes = malloc(obj->numNodes * sizeof(Node*));
	obj->nodes = calloc(obj->numNodes, sizeof(FxsObjFile2Node));

	nodes[0] = file->root;

	for (i = 0; i < obj->numNodes; i++) {
		 MapNodeToFxsObjFile2Node(obj->nodes, i, nodes, &j);    
	}	
}

static FxsObjFile2* FxsObjFile2CreateWithFile(File *file)
{
	FxsObjFile2 *obj;

	assert(file);

	obj = calloc(1, sizeof(FxsObjFile2));
	
	if (!obj) {
	   	return NULL; 
	}

	FxsObjFile2InitHeader(obj, file);
	FxsObjFile2InitGeometry(obj, file);
	FxsObjFile2InitSceneGraph(obj, file);
	
	return obj;
}

/***** FxsObjFile2 public interface definition *******************************/

FxsObjFile2* FxsObjFile2Create(const char *filename)
{
	FxsObjFile2 *obj = NULL;
	FILE *f = NULL;
	char line[MAX_STRLEN];
	File *of;

	f = fopen(filename, "r");

	if (!f) {
	    return NULL;
	}

	obj = calloc(1, sizeof(FxsObjFile2));

	if (!obj) {
	    fclose(f);
	}

	of = FileCreate();

	while (fgets(line, MAX_STRLEN, f)) {
	   	FileUpdateWithLine(of, line);
	}
    
	/* get rid of empty sub-trees in the scene graph */
	NodeTrim(of->root);

    /* convert tmp obj file to the final one */
	obj = FxsObjFile2CreateWithFile(of);

	FileDestroy(of);
	fclose(f);
	return obj;

error:
	fclose(f);
	FileDestroy(of);
	FxsObjFile2Destroy(&obj);
	return NULL;
}

void FxsObjFile2Destroy(FxsObjFile2 **file)
{
	FxsObjFile2* obj;

	if (!file || !(*file)) {
	    return;
	}

	FREE(obj->positions);
	FREE(obj->normals);
	FREE(obj->texCoords);
	FREE(obj->faces);
	FREE(obj->nodes);

	free(obj);
	*file = NULL;
}
