#include "Texture.h"

/* let's use the mighty stb_image library! */
extern unsigned char *stbi_load(
	char const *filename, 
	int *x, 
	int *y, 
	int *comp, 
	int req_comp
);

extern void stbi_image_free(void *retval_from_stbi_load);

GLuint FxsOpenGLTexture2DCreate(const char* filename)
{
	GLuint tex = 0;
	int w, h, n;
	unsigned char* data = NULL;

	if (!filename) 
	{
	    return 0;
	}

	data = stbi_load(filename, &w, &h, &n, 0);

	if (!data) 
	{
	    return 0;
	}

	glGenTextures(1, &tex);
	
	if (!tex) 
	{
		goto error; 
	}

	glBindTexture(GL_TEXTURE_2D, tex);

	if (n == 3) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
	}
	else if (n == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
	}
	else
	{
		goto error;	
	}
	
    /* give our texture a default configuration */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (GL_NO_ERROR != glGetError()) 
	{
	   	goto error; 
	}

	stb_image_free(data);
	return tex;

error:

	if (tex) 
	{
	    glDeleteTextures(1, &tex);
	}

	if (data) 
	{
	    stb_image_free(data);
	}
	
	return 0;
}
