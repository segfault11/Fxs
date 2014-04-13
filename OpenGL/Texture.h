/*
 * Simple texture loader utilities based on the great stbi library 
 * (http://www.nothings.org/stb_image.c).
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
#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef FXS_USE_GLEW_HEADER
    #include <GL/glew.h>
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include "glcorearb.h"
#endif

/*
** Loads a image and creates a 2d texture from it.
** NOTES: 
** 	- binds the created texture to the current texture unit.
**  - regardless of its source componenents, the texture is stored as a 
**    RGBA texture on the device
**  - The input image is assumed to be either RGB or RGBA
**  - Delete the texture as usual with glDeleteTextures(...)
**
** @param filename Filename of the source image.
** @return A handle to the texture or 0 if s.th. went wrong.
*/ 
GLuint FxsOpenGLTexture2DCreate(const char* filename);


#ifdef __cplusplus
}
#endif

#endif /* end of include guard: TEXTURE_H */
