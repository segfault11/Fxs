/*
 * Utility functions for OpenGL Programs
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
#ifndef PROGRAM_H
#define PROGRAM_H

#ifdef __cplusplus
extern "C"
{
#endif

#define GL_GLEXT_PROTOTYPES 1
#include "glcorearb.h"

/*
** Attaches a shader to the program.
**
** @param program Program the shader is attached to.
** @param type Type of the shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc.) 
** @param source Source of the shader as a zero terminated c array.
** 
** @return 0 in case of failure 1 otherwise.
*/ 
int FxsOpenGLProgramAttachShaderWithSource(
	GLuint program, 
	GLenum type, 
	const char* source
);


/*
** Attaches a shader to the program.
**
** @param program Program the shader is attached to.
** @param type Type of the shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER etc.) 
** @param file Filename of the shader to be attached to the program. 
** 
** @return 0 in case of failure 1 otherwise.
*/ 
int FxsOpenGLProgramAttachShaderWithFile(
	GLuint program, 
	GLenum type, 
	const char* file
);

/*
** Links the openglgl program.
**
** @return 0 in case of failure 1 otherwise.
*/ 
int FxsOpenGLProgramLink(GLuint program);

/*
** sets an integer scalar for the program
*/ 
int FxsOpenGLProgramUniform1i(
	GLuint program, 
	const char* name, 
	GLint value
);

/*
** sets a float scalar for the program
*/ 
int FxsOpenGLProgramUniform1f(
	GLuint program, 
	const char* name, 
	GLfloat value
);

/*
** sets a 2d vector for the program
*/ 
int FxsOpenGLProgramUniform2f(
	GLuint program, 
	const char* name, 
	GLfloat v0, 
	GLfloat v1 
);

/*
** sets a 3d vector for the program
*/ 
int FxsOpenGLProgramUniform3f(
	GLuint program, 
	const char* name, 
	GLfloat v0, 
	GLfloat v1, 
	GLfloat v2
);

/*
** sets a matrix for the program
*/ 
int FxsOpenGLProgramUniformMatrix4(
    GLuint program,
    const char* name, 
    const GLfloat* value, 
    GLboolean transpose	
);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard: PROGRAM_H */
