#include "ObjMtlFile.h"
#include <Fxs/Dictionary/Dictonary>
#include <string.h>

#define MAX_STR_LENGTH 1024

typedef struct FxsObjMaterial_
{
	char** components;
	unsigned int numComponents;	
}
FxsObjMaterial;

typedef struct FxsObjMtlFile_
{
	FxsDictionaryPtr dict;
	FxsObjMaterial* materials;
	unsigned int numMaterials;
}
FxsObjMtlFile;

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

static char* Trim(char* line)
{
	int i = 0;
	char* l = line;
	char* end = NULL;

	while(isspace(line[i]))
	{
		i++;
	}

	l = line + i;

	end = l + strlen(l) - 1;

	while (end > l && isspace(*end))
	{
		end--;
	}

	*(end + 1) = '\0';

	return l;
}

static int CountWords(char* line)
{
	int i = 0;
	char* str = NULL;

	str = strtok(line, " \t");

	while (str)
	{
		str = strtok(NULL, " \t");
		i++;
	}

	return i;
}

static int CountMaterials(FILE* f)
{
	char line[MAX_STR_LENGTH];
	char* pline = NULL;
	unsigned int cnt = 0;

	while (fgets(line, MAX_STR_LENGTH, f))
	{
		if (IsBlankLine(line))
		{
			continue;
		}

		pline = Trim(line);

		if (strstr(line, "newmtl ") == line)
		{
			if (sscanf(line, "newmtl %s", &mtlname) == 1) 
			{
			    cnt++;
			}		
		}
	}

	return cnt;
}

int FxsObjMtlFileCreateWithFile(
	FxsObjMtlFilePtr* mtlfile, 
	const char* filename
)
{
	FILE* f = NULL;
	char line[MAX_STR_LENGTH];
	char mtlname[MAX_STR_LENGTH];
	char* pline = NULL;
	int numWords = 0;

	f = fopen(filename, "r");

	if (!f)
	{
		return 0;
	}

	*mtlfile = (FxsObjMtlFilePtr)malloc(sizeof(FxsObjMtlFile));

	if (!mtlfile) 
	{
	    goto error;
	}

	memset(*mtlfile, 0, sizeof(FxsObjMtlFile));

	(*mtlfile)->numMaterials = CountMaterials(file);

	if ((*mtlfile)->numMaterials == 0) 
	{
		goto error;
	}

	(*mtlfile)->materials = (FxsObjMaterial*)malloc(
			sizeof(FxsObjMaterial)*(*mtlfile)->numMaterials
		);

	if (!(*mtlfile)->materials) 
	{
		goto error;	  
	}

	memset(
		(*mtlfile)->materials, 
		0, 
		sizeof(FxsObjMaterial)*(*mtlfile)->numMaterials
	);

	if (!FxsDictionaryCreateWithTableSize(
			&(*mtlfile)->dict,
			2*(*mtlfile)->numMaterials
		)
	)
	{
		goto error;	
	}

	while (fgets(line, MAX_STR_LENGTH, f)) 
	{
	    if (strstr(line, "newmtl ") == line)
		{
			if (sscanf(line, "newmtl %s", &mtlname) != 1) 
			{
			    goto error;
			}		

			if (IsBlankLine(line)) 
			{
			    continue;
			}

			pline = Trim(line);

			numWords = CountWords(pline);
		}
	}

	fclose(f);
	return 1;

error:

	fclose(f);
	return 0;
}
