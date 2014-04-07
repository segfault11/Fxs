#include "ObjMtlFile.h"
#include "../Dictionary/Dictionary.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <float.h> 

#define MAX_STR_LEN 1024

#define ERR_MSG(X) printf("In file %s line\n\t%s\n", __FILE__, __LINE__, X);

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

static int IsBlankLine(char* line)
{
	int i = 0;

	while (line[i] != EOF && line[i] != '\0' && line[i] != '\r' && line[i] != '\n')
	{
		if (!isspace(line[i]))
		{
		    return 0;
		}

		i++;
	}

	return 1;
}

/*******************************************************************************
** FxsObjMaterialAttrib stuff
*******************************************************************************/
typedef struct FxsObjMaterialAttribute_ 
{
	char* name;
   	char** elements;
	unsigned int numElements;
}
FxsObjMaterialAttribute;

/*
** counts # of words seperated by ' ' or  '\t'
*/ 
static unsigned int CountWords(char* line)
{
    char tmpline[MAX_STR_LEN];
	unsigned int i = 0;
	char* str = NULL;
    
    strcpy(tmpline, line);

	str = strtok(tmpline, " \t");

	while (str)
	{
		str = strtok(NULL, " \t");
		i++;
	}

	return i;
}

FxsObjMaterialAttribute* FxsObjMaterialAttributeCreateWithLine(char* line)
{
	unsigned int numWords = CountWords(line);
	FxsObjMaterialAttribute* attr = NULL;	
	char* str = NULL;
    char tmpline[MAX_STR_LEN];
	int i = 0, j = 0;	

	assert(numWords > 0);
    strcpy(tmpline, line);

	attr = (FxsObjMaterialAttribute*)malloc(sizeof(FxsObjMaterialAttribute));

	if (!attr) 
	{
	    return NULL;
	}

	memset(attr, 0, sizeof(FxsObjMaterialAttribute));



	/* get name of the element */
	str = strtok(tmpline, " \t");
	attr->name = (char*)malloc(strlen(str) + 1);

	if (!attr->name) 
	{
		free(attr);
		return NULL; 
	}

    strcpy(attr->name, str);

	if (numWords == 1) /* attribute does not have any elements */
	{
	    return attr;
	}
	
	/* read the elements */
	attr->numElements = numWords - 1;
	attr->elements = (char**)malloc(sizeof(char*)*attr->numElements);

	if (!attr->elements) 
	{
	    free(attr->name); 
		free(attr);
		return NULL;
	}

	while (1)
	{
		str = strtok(NULL, " \t");
        
        if (!str)
        {
            break;
        }
        
		attr->elements[i] = (char*)malloc(strlen(str) + 1);

		if (!attr->elements[i]) 
		{
			for (i = 0; i < j; i++) 
			{
			 	free(attr->elements[i]);
				free(attr->name);
				free(attr);
			}
			return NULL;
		}

		strcpy(attr->elements[i], str);

		i++;	    
	}
	
	return attr;
}

void FxsObjMaterialAttributeDestroy(FxsObjMaterialAttribute* attr)
{
	int i = 0;

	if (attr->elements) 
	{
		for (i = 0; i < attr->numElements; i++) 
		{
	    	free(attr->elements[i]);
		}
	
		free(attr->elements);
	}

	free(attr->name);
	free(attr);
}

/*******************************************************************************
** FxsObjMaterial stuff
*******************************************************************************/
typedef struct FxsObjMaterial_
{
	FxsDictionaryPtr dict; /* stores attributes */
}
FxsObjMaterial;

FxsObjMaterialPtr FxsObjMaterialCreate()
{
	FxsObjMaterialPtr mat = (FxsObjMaterialPtr)malloc(sizeof(FxsObjMaterial));

	if (!mat) 
	{
		return NULL; 
	}

	mat->dict = FxsDictionaryCreateWithTableSize(10);
	
	if (!mat->dict) 
	{
		free(mat);
	    return NULL;
	}

	return mat;
}

void FxsObjMaterialDestroy(FxsObjMaterialPtr mat)
{
	FxsListPtr keyList = NULL;
	FxsListIteratorPtr attrIterator = NULL;
    FxsObjMaterialAttribute* attr = NULL;
	const char* key;
    
	keyList = FxsDictionaryGetKeys(mat->dict);
	attrIterator = FxsListIteratorCreate(
			keyList, 
			FXS_LIST_FRONT, 
			FXS_LIST_FRONT_TO_BACK
		);

	while (FxsListIteratorHasNext(attrIterator))
	{
        key = FxsListIteratorNext(attrIterator);
    
        if (FxsDictionaryContains(mat->dict, key))
        {
            attr = FxsDictionaryFind(mat->dict, key);
            FxsObjMaterialAttributeDestroy(attr);
        }
	}

	FxsListIteratorDestroy(&attrIterator);	
	FxsDictionaryDestroy(&mat->dict);
	free(mat);
}

unsigned int FxsObjMaterialGetAttributeSize(
	FxsObjMaterialPtr material,
	const char* name
)
{
	FxsObjMaterialAttribute* attr = NULL;

	if (!material) 
	{
	   	return 0; 
	}

	if (!FxsDictionaryContains(material->dict, name)) 
	{
	    return 0;
	}

	attr = FxsDictionaryFind(material->dict, name);

	return attr->numElements;
}

const char* FxsObjMaterialGetAttributeElementAsString(
	FxsObjMaterialPtr material,
	const char* name, 
	unsigned int idx
)
{
	FxsObjMaterialAttribute* attr = NULL;

	if (!material) 
	{
	   	return NULL; 
	}

	if (!FxsDictionaryContains(material->dict, name)) 
	{
	    return NULL;
	}

	attr = FxsDictionaryFind(material->dict, name);

	if (idx >= attr->numElements) 
	{
	    return NULL;
	}

	return attr->elements[idx];
}

float FxsObjMaterialGetAttributeElementAsFloat(
	FxsObjMaterialPtr material,
	const char* name, 
	unsigned int idx
)
{
	FxsObjMaterialAttribute* attr = NULL;

	if (!material) 
	{
	   	return FLT_MAX; 
	}

	if (!FxsDictionaryContains(material->dict, name)) 
	{
	    return FLT_MAX;
	}

	attr = FxsDictionaryFind(material->dict, name);

	if (idx >= attr->numElements) 
	{
	    return FLT_MAX;
	}

	return atof(attr->elements[idx]);
}

int FxsObjMaterialHasAttribute(FxsObjMaterialPtr material, const char* name)
{
    return FxsDictionaryContains(material->dict, name);
}

/*******************************************************************************
** FxsObjMtlFile stuff
*******************************************************************************/
typedef struct FxsObjMtlFile_
{
	FxsDictionaryPtr dict;
}
FxsObjMtlFile;

FxsObjMtlFilePtr FxsObjMtlFileCreateWithFile(
	const char* filename
)
{
	FILE* f = NULL;
	char line[MAX_STR_LEN];
	char mtlname[MAX_STR_LEN];
	FxsObjMtlFilePtr mtlfile = NULL;
	FxsObjMaterialPtr currMat = NULL;
	FxsObjMaterialAttribute* currElem = NULL;

	f = fopen(filename, "r");

	if (!f)
	{
		return NULL;
	}

	mtlfile = (FxsObjMtlFilePtr)malloc(sizeof(FxsObjMtlFile));

	if (!mtlfile) 
	{
	    goto error;
	}

	memset(mtlfile, 0, sizeof(FxsObjMtlFile));

	mtlfile->dict = FxsDictionaryCreateWithTableSize(20);

	if (!mtlfile->dict)
	{
		goto error;	
	}

	while (fgets(line, MAX_STR_LEN, f)) 
	{
		if (IsBlankLine(line)) 
		{
			continue;
		}

		PreProcessLine(line);

		/* ignore comments */
		if (line[0] == '#') 
		{
			continue;
		}

	    if (strstr(line, "newmtl ") == line) /* add a new material */
		{
			if (sscanf(line, "newmtl %s", mtlname) != 1) 
			{
			    goto error;
			}		
		
			currMat = FxsObjMaterialCreate();

			if (!currMat) 
			{
			    goto error;
			}
	
			FxsDictionaryInsert(mtlfile->dict, mtlname, currMat);
		}
		else /* add a new attribute to the current material */
		{
			currElem = FxsObjMaterialAttributeCreateWithLine(line);	

			if (!currElem) 
			{
				goto error;	 
			}

			FxsDictionaryInsert(currMat->dict, currElem->name, currElem);
		}
	}

	fclose(f);
	return mtlfile;

error:
	FxsObjMtlFileDestroy(&mtlfile);
	fclose(f);
	return NULL;
}

void FxsObjMtlFileDestroy(FxsObjMtlFilePtr* mtlfile)
{
	FxsListPtr keyList = NULL;
	FxsListIteratorPtr matIterator = NULL;
    FxsObjMaterialPtr mat = NULL;
    char* key = NULL;

	if (!mtlfile || !(*mtlfile)) 
	{
	    return;
	}

	if ((*mtlfile)->dict)
	{
		keyList = FxsDictionaryGetKeys((*mtlfile)->dict);

		matIterator = FxsListIteratorCreate(
				keyList,
				FXS_LIST_FRONT,
				FXS_LIST_FRONT_TO_BACK
			);
	
		while (FxsListIteratorHasNext(matIterator))
		{
            key = FxsListIteratorNext(matIterator);
            
            if (FxsDictionaryContains((*mtlfile)->dict, key))
            {
                mat = FxsDictionaryFind((*mtlfile)->dict, key);
                FxsObjMaterialDestroy(mat);
            }
		}

		FxsListIteratorDestroy(&matIterator);
		FxsDictionaryDestroy(&(*mtlfile)->dict);	
	}

	free(*mtlfile);
	*mtlfile = NULL;
}

FxsObjMaterialPtr FxsObjMtlFileGetMaterialWithName(
	FxsObjMtlFilePtr mtlfile,
	const char* name
)
{
	if (!mtlfile) 
	{
	    return NULL;
	}

	if (!FxsDictionaryContains(mtlfile->dict, name)) 
	{
	    return NULL;
	}

	return FxsDictionaryFind(mtlfile->dict, name);
}
