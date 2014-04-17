/*
 * A simple implementation of an image based on the stb library 
 * (http://nothings.org)
 * Copyright (C) 2014 Arno in Wolde Luebke 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef IMAGE_H
#define IMAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	unsigned char* data; /* NOTE: byte alignment is always 1 */
	int width;
	int height;
	int numChannels;
}
FxsImage;

typedef FxsImage* FxsImagePtr;

/*
** Loads the the image from a file. Returns NULL if the method fails.
*/ 
FxsImagePtr FxsImageCreateWithFilename(const char* filename); 

/*
** Releases the image.
*/ 
void FxsImageDestroy(FxsImagePtr* image);

/*
** Sets a pixel in the image.
**
** @param color The color of the image. "color" is either a 1, 2, 3, 4D 
**              depending on the numChannels in the images struct.
** @return 0 if the method fails, 1 otherwise.
*/ 
int FxsImageSet(FxsImagePtr image, int i, int j, unsigned char* color);

/*
** Gets the color of a pixel (i, j). Returns a pointer to a char array. 
** Depending in the numChannels in the image the array should be interpreted
** as a 1,2,3 or 3 dimensional image.
*/ 
const unsigned char* FxsImageGet(FxsImagePtr image, int i, int j);

/*
** Saves the image to a file.
*/ 
int FxsImageSave(FxsImagePtr image, const char* filename);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: IMAGE_H */
