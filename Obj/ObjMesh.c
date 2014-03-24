#include "ObjMesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Fxs/Dictionary/Dictionary.h>

#define MAX_STR_LENGTH 1024

#define ERR_MSG(X) printf("In file: %s line: %d\n\t%s\n", __FILE__, __LINE__, X);
static char errMsg[1024];

void ReadMaterialName(char* materialName, char* line)
{
	int i = 0;
	char c = 0;	

	while (1)
	{
		c = line[strlen("usemtl ") + i];

		if (c == EOF || c == '\r' || c == '\n' || c == '\0')
		{
			break;
		}

		materialName[i] = c;

		i++;
	}

	materialName[i] = '\0';
}

static int LoadFile(FxsObjMesh* mesh, FILE* file, FxsDictionaryPtr matDict)
{
    FxsObjNode* currentNode = NULL;
    FxsObjNode* currentObjectNode = NULL;
	FxsObjFace* currentFace = NULL;
	FxsVector3* currentVertexData = NULL;
	FxsVector2* currentTexCoord = NULL;
    int currentMaterial = -1;
	unsigned int numFacesLoaded = 0;
	unsigned int numPositionsLoaded = 0;
	unsigned int numNormalsLoaded = 0;
	unsigned int numTexCoordsLoaded = 0;
	unsigned int numObjectsLoaded = 0;
    unsigned int numGroupsLoaded = 0;
	char line[MAX_STR_LENGTH];
    char str[MAX_STR_LENGTH];
	int lineNr = 1;
	int scres = 0;
    size_t strl = 0;
	int i = 0;
    int* matId = NULL;

	currentNode = &mesh->root;  

	while (fgets(line, MAX_STR_LENGTH, file))
	{
        if (line == strstr(line, "mtllib "))
        {
            scres = sscanf(line, "mtllib %s", str);
            
			if (scres == 1)
			{
                strl = strlen(str) + 1;
			    mesh->mtlname = (char*)malloc(strl);
                
                if (!mesh->mtlname)
                {
                    ERR_MSG("Error: Alloc failed");
                    return 0;
                }
                
                strcpy(mesh->mtlname, str);
                continue;
			}
            
            sprintf(errMsg, "Error: Invalid mtl file declaration in line %d of the source file %s. ", lineNr, mesh->name);
			ERR_MSG(errMsg);

			return 0;
        }
    
		if (line == strstr(line, "f "))  /* read in faces */
		{
			currentNode->numFaces++;
			currentFace = &mesh->faces[numFacesLoaded];
	  		numFacesLoaded++;

			currentFace->matIdx = 0;
			currentFace->tc0 = 0;
			currentFace->tc1 = 0;
			currentFace->tc2 = 0;
			currentFace->n0 = 0;
			currentFace->n1 = 0;
			currentFace->n2 = 0;
            currentFace->matIdx = currentMaterial;

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
			    continue;
			}
			

			sprintf(errMsg, "Error: Invalid face declaration in line %d of the source file %s. ", lineNr, mesh->name);
			ERR_MSG(errMsg);

			return 0;	
		}		
		
		if (strstr(line, "v ") == line)  /* load positions */
		{
			currentVertexData = &mesh->positions[numPositionsLoaded];
			numPositionsLoaded++;
			
			scres = sscanf(
					line, "v %f %f %f", 
					&currentVertexData->x,
					&currentVertexData->y,
					&currentVertexData->z
				);

			if (scres == 3) 
			{
			    continue;
			}

			sprintf(errMsg, "Error: Invalid vertex declaration in line %d of the source file %s. ", lineNr, mesh->name);
			ERR_MSG(errMsg);
			
			return 0;
		}

		if (strstr(line, "vn ") == line)  /* load normals */
		{
			currentVertexData = &mesh->normals[numNormalsLoaded];
			numNormalsLoaded++;
			
			scres = sscanf(
					line, "vn %f %f %f", 
					&currentVertexData->x,
					&currentVertexData->y,
					&currentVertexData->z
				);

			if (scres == 3) 
			{
			    continue;
			}

			sprintf(errMsg, "Error: Invalid vertex normal declaration in line %d of the source file %s. ", lineNr, mesh->name);
			ERR_MSG(errMsg);
			
			return 0;
		}

		if (strstr(line, "vt ") == line)  /* load tex coordinates */
		{
			currentTexCoord = &mesh->texCoords[numTexCoordsLoaded];
			numTexCoordsLoaded++;
			
			scres = sscanf(
					line, "vt %f %f", 
					&currentTexCoord->x,
					&currentTexCoord->y
				);

			if (scres == 3)
			{
			    continue;
			}

			sprintf(errMsg, "Error: Invalid vertex tex coordinate declaration in line %d of the source file %s. ", lineNr, mesh->name);
			ERR_MSG(errMsg);
			
			return 0;
		}
        
		if (strstr(line, "o ") == line)  /* load objects */
		{
            mesh->nodes[numObjectsLoaded].type = FXS_OBJ_OBJECT;
            mesh->nodes[numObjectsLoaded].childId = mesh->root.numChildren + numGroupsLoaded;
            mesh->nodes[numObjectsLoaded].numChildren = 0;
            mesh->nodes[numObjectsLoaded].faceId = numFacesLoaded;
    
            scres = sscanf(line, "o %s", str);
            
            if (scres != 1)
            {
                return 0;
            }
            
            strl = strlen(str) + 1;
            
            mesh->nodes[numObjectsLoaded].name = (char*)malloc(strl);
            
            if (!mesh->nodes[numObjectsLoaded].name)
            {
                ERR_MSG("Alloc failed");
            }
            
            strcpy(mesh->nodes[numObjectsLoaded].name, str);
            
            currentNode = &mesh->nodes[numObjectsLoaded];
            currentObjectNode = &mesh->nodes[numObjectsLoaded];
            
            numObjectsLoaded++;
            
            continue;
		}
        
		if (strstr(line, "g ") == line)  /* load objects */
		{
            if (!currentObjectNode)
            {
                sprintf(errMsg, "Error: Invalid group declaration in line %d of the source file %s. Groups need to belong to an object.", lineNr, mesh->name);
                ERR_MSG(errMsg);
                return 0;
            }
            
            currentObjectNode->numChildren++;
            
            mesh->nodes[mesh->root.numChildren + numGroupsLoaded].type = FXS_OBJ_GROUP;
            mesh->nodes[mesh->root.numChildren + numGroupsLoaded].childId = -1;
            mesh->nodes[mesh->root.numChildren + numGroupsLoaded].numChildren = 0;
            mesh->nodes[mesh->root.numChildren + numGroupsLoaded].faceId = numFacesLoaded;
            
            scres = sscanf(line, "g %s", str);
            
            if (scres != 1)
            {
                return 0;
            }
            
            strl = strlen(str) + 1;
            
            mesh->nodes[mesh->root.numChildren + numGroupsLoaded].name = (char*)malloc(strl);
            
            if (!mesh->nodes[mesh->root.numChildren + numGroupsLoaded].name)
            {
                ERR_MSG("Alloc failed");
            }
            
            strcpy(mesh->nodes[mesh->root.numChildren + numGroupsLoaded].name, str);
            
            currentNode = &mesh->nodes[mesh->root.numChildren + numGroupsLoaded];
            
            numGroupsLoaded++;
            
            continue;
		}
        
        if (strstr(line, "usemtl ") == line)
        {
            ReadMaterialName(str, line);
            matId = FxsDictionaryFind(matDict, str);
            
            if (!matId)
            {
                return 0;
            }
            
            currentMaterial = *matId;
            
            if (!mesh->materials[currentMaterial])
            {
                strl = strlen(str) + 1;
                mesh->materials[currentMaterial] = (char*)malloc(strl);
                
                if (!mesh->materials[currentMaterial])
                {
                    ERR_MSG("Error: alloc failed");
                    return 0;
                }
                
                strcpy(mesh->materials[currentMaterial], str);
            }
        }
        
		lineNr++;
	}
    
    /* convert face indices so that that resemble array indices, 
	** i.e. substract 1 
	*/
	for (i = 0; i < mesh->numFaces; i++)
	{
		mesh->faces[i].p0--;
		mesh->faces[i].p1--;
		mesh->faces[i].p2--;
		mesh->faces[i].n0--;
		mesh->faces[i].n1--;
		mesh->faces[i].n2--;
		mesh->faces[i].tc0--;
		mesh->faces[i].tc1--;
		mesh->faces[i].tc2--;
	}
    
    /* */
    for (i = 0; i < mesh->numNodes; i++)
    {
        if (!mesh->nodes[i].numChildren)
        {
            mesh->nodes[i].childId = -1;
        }

        if (!mesh->nodes[i].numFaces)
        {
            mesh->nodes[i].faceId = -1;
        }

    }
    
    if (!mesh->root.numChildren)
    {
        mesh->root.childId = -1;
    }

    if (!mesh->root.numFaces)
    {
        mesh->root.faceId = -1;
    }
    

	return 1;
}


int FxsObjMeshCreateWithFile(FxsObjMesh** mesh, const char* filename)
{
	FILE* f = NULL;
	char line[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];
	unsigned int numFaces = 0;
    unsigned int numPositions = 0;
    unsigned int numNormals = 0;
    unsigned int numTexCoords = 0;
    unsigned int numNodes = 0;
    unsigned int numMaterials = 0;
    int succ = 1;
	size_t strl = 0;
    FxsDictionaryPtr dict;
    
    if (!FxsDictionaryCreateWithTableSize(&dict, 100))
    {
        return 0;
    }

    if (!mesh)
    {
        return 0;
    }
    
	f = fopen(filename, "r"); 

	if (!f) 
	{
	    return 0;
	}

    *mesh = (FxsObjMesh*)malloc(sizeof(FxsObjMesh));
    memset(*mesh, 0, sizeof(FxsObjMesh));

    if (!(*mesh))
    {
		fclose(f);
        return 0;
    }
   
	strl = strlen(filename); 
	(*mesh)->name = (char*)malloc(strl + 1);

	if (!(*mesh)->name)
	{
		succ = 0;
		goto cleanup;
	}

	strcpy((*mesh)->name, filename); 
 
    /* count # of faces, vertices, etc. */
    while (fgets(line, MAX_STR_LENGTH, f))
    {
        if (strstr(line, "usemtl ") == line)
        {
            ReadMaterialName(str, line);
            
            /* if we have not found the same material before */
            if (!FxsDictionaryFind(dict, str))
            {
                /* save the materials index to the dict */
                FxsDictionaryInsert(dict, str, &numMaterials, sizeof(unsigned int));
                numMaterials++; /* increase the # of materials */
            }
        }
    
        if (strstr(line, "f ") == line)
        {
            numFaces++;
        }
        else if (strstr(line, "v ") == line)
        {
            numPositions++;
        }
        else if (strstr(line, "vn ") == line)
        {
            numNormals++;
        }
        else if (strstr(line, "vt ") == line)
        {
            numTexCoords++;
        }
        else if (strstr(line, "g ") == line)
        {
            numNodes++;
        }
        else if (strstr(line, "o ") == line)
        {
            (*mesh)->root.numChildren++;
            numNodes++;
        }
        else if (strstr(line, "mtllib "))
        {
        
        }
    }

    /* allocate the data the obj. file holds */
    if (numFaces)
    {
        (*mesh)->numFaces = numFaces;
        (*mesh)->faces = (FxsObjFace*)malloc(numFaces*sizeof(FxsObjFace));

        if (!(*mesh)->faces)
        {
            succ = 0;
			goto cleanup;
        }

		memset((*mesh)->faces, 0, sizeof(FxsObjFace)*numFaces);
    }

    if (numPositions)
    {
        (*mesh)->numPositions = numPositions;
        (*mesh)->positions = (FxsVector3*)malloc(numPositions*sizeof(FxsVector3));
        
        if (!(*mesh)->positions)
        {
            succ = 0;
			goto cleanup;
        }

		memset((*mesh)->positions, 0, sizeof(FxsVector3)*numPositions);
    }
    
    if (numNormals)
    {
        (*mesh)->numNormals = numNormals;
        (*mesh)->normals = (FxsVector3*)malloc(numPositions*sizeof(FxsVector3));

        
        if (!(*mesh)->normals)
        {
            succ = 0;
			goto cleanup;
        }
		
		memset((*mesh)->normals, 0, sizeof(FxsVector3)*numNormals);
    }

	if (numTexCoords) 
	{
	    (*mesh)->numTexCoords = numTexCoords;
		(*mesh)->texCoords = (FxsVector2*)malloc(sizeof(FxsVector2)*numTexCoords);
		
		if (!(*mesh)->texCoords)
		{
			succ = 0;
			goto cleanup;
		}

		memset((*mesh)->texCoords, 0, sizeof(FxsVector2)*numTexCoords);
	}

    if (numNodes)
    {
        (*mesh)->numNodes = numNodes;
        (*mesh)->nodes = (FxsObjNode*)malloc(numNodes*sizeof(FxsObjNode));
        
        if (!(*mesh)->nodes)
        {
            succ = 0;
			goto cleanup;
        }

		memset((*mesh)->nodes, 0, sizeof(FxsVector3)*numNodes);
    }

    if (numMaterials)
    {
        (*mesh)->numMaterials = numMaterials;
        (*mesh)->materials = (char**)malloc(sizeof(char*)*numMaterials);
        
        if (!(*mesh)->materials)
        {
            succ = 0;
			goto cleanup;
        }
        
        memset((*mesh)->materials, 0, sizeof(char*)*numMaterials);
    }

    /* load data */
    rewind(f);
    
    if (!LoadFile(*mesh, f, dict))
    {
        succ = 0;
        goto cleanup;
    }
    
    /* clean up */
cleanup:
    fclose(f);
    FxsDictionaryDestroy(&dict);
    
    if (!succ)
    {
        FxsObjMeshDestroy(mesh);
        return 0;
    }
    
	return 1;
}

void FxsObjMeshDestroy(FxsObjMesh** mesh)
{
	int i = 0;

	if (!mesh || !(*mesh)) 
	{
	    return;
	}

	if ((*mesh)->faces)
	{
		free((*mesh)->faces);
	}
	
	if ((*mesh)->positions)
	{
		free((*mesh)->positions);
	}
	
	if ((*mesh)->normals)
	{
		free((*mesh)->normals);
	}
	
	if ((*mesh)->texCoords)
	{
		free((*mesh)->texCoords);
	}

	if ((*mesh)->nodes) 
	{
		for (i = 0; i < (*mesh)->numNodes; i++)
		{
			if ((*mesh)->nodes[i].name) 
			{
				free((*mesh)->nodes[i].name);
			}
		}
	}
    
    if ((*mesh)->mtlname)
    {
        free((*mesh)->mtlname);
    }
    
    if ((*mesh)->materials)
    {
        for (int i = 0; i < (*mesh)->numMaterials; i++)
        {
            if ((*mesh)->materials[i])
            {
                free((*mesh)->materials[i]);
            }
        }
        
        free((*mesh)->materials);
    }
    
    free(*mesh);
    
	*mesh = NULL;
}
