#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"



FxsImagePtr FxsImageCreate(int width, int height, int numChannels)
{
	FxsImagePtr img = malloc(sizeof(FxsImage));

	if (!img) {
	    return NULL;
	}

	img->width = width;
	img->height = height;
	img->numChannels = numChannels;
	img->data = malloc(sizeof(unsigned char) * width * height * numChannels);

	if (!img->data) {
	    free(img);
		return NULL;
	}

	return img;
}

FxsImagePtr FxsImageCreateWithFilename(const char* filename)
{
	FxsImagePtr img = malloc(sizeof(FxsImage));

	if (!img) {
	    return NULL;
	}

	img->data = stbi_load(filename, &img->width, &img->height, &img->numChannels, 0);

	if (!img->data) {
		return NULL;
	}

	return img;
}

void FxsImageDestroy(FxsImagePtr* image)
{
	if (!image || !(*image)) {
	   	return; 
	}

	if ((*image)->data) {
	   	stbi_image_free((*image)->data);
	}

	free(*image);
	*image = NULL;
}

int FxsImageSave(FxsImagePtr image, const char* filename)
{	
	size_t len = 0;
	const char* suff = NULL;

	if (!filename) {
		return 0;
	}

	if ((len = strlen(filename)) < 4) {
	    return 0;
	}

	suff  = filename + len - 4;

	if (strcmp(suff, ".bmp") == 0) {
		stbi_write_bmp(filename, image->width, image->height, 
			image->numChannels, image->data);
	} else if (strcmp(suff, ".tga") == 0) {
		stbi_write_tga(filename, image->width, image->height, 
			image->numChannels, image->data);
	} else if (strcmp(suff, ".png") == 0) {
		stbi_write_png(filename, image->width, image->height, 
			image->numChannels, image->data, 0);
	} else {
		return 0;
	}
	

	return 1;
}

int FxsImageSet(FxsImagePtr image, int i, int j, unsigned char* color)
{
	size_t idx = 0;

	if (!image || i >= image->width || i < 0 
		|| j >= image->height || j < 0 || !color) {
	   	return 0; 
	}

	idx = (j*image->width + i)*image->numChannels;

	memcpy(image->data + idx, color, image->numChannels);

	return 1;
}

const unsigned char* FxsImageGet(FxsImagePtr image, int i, int j)
{
	size_t idx = 0;

	if (!image || i >= image->width || i < 0 
		|| j >= image->height || j < 0) {
	   	return 0; 
	}

	idx = (j*image->width + i)*image->numChannels;

	return image->data + idx;
}

