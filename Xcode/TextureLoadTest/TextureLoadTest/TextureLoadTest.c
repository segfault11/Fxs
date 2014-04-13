//
//  TextureLoadTest.c
//  TextureLoadTest
//
//  Created by Arno in Wolde Luebke on 08.04.14.
//  Copyright (c) 2014 Arno in Wolde Luebke. All rights reserved.
//

#include <stdio.h>
#include <Fxs/OpenGL/Program.h>
#include <Fxs/OpenGL/Texture.h>
#include <assert.h>

#define TO_STRING(X) #X

static char* vertexShader =
    "#version 150\n"
TO_STRING(

    in vec2 position;
    
    void main()
    {
        gl_Position = vec4(0.5*position, 0.0, 1.0);
    }
);

static char* fragmentShader =
    "#version 150\n"
TO_STRING(

    out vec4 fragOut;
    
    void main()
    {
        fragOut = vec4(1.0, 0.0, 0.0, 1.0);
    }
);

static float vertices[] = {
    -1.0, -1.0,
    1.0, -1.0,
    -1.0, 1.0,
    1.0, -1.0,
    1.0, 1.0,
    -1.0, 1.0
};

static GLuint program;
static GLuint vao;
static GLuint vbo;
static GLuint texture;

void Init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    program = glCreateProgram();
    FxsOpenGLProgramAttachShaderWithSource(program, GL_VERTEX_SHADER, vertexShader);
    FxsOpenGLProgramAttachShaderWithSource(program, GL_FRAGMENT_SHADER, fragmentShader);
    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragOut");
    FxsOpenGLProgramLink(program);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, 0);
    
    texture = FxsOpenGLTexture2DCreate("Mezzanine.jpg");
    
    if (!texture)
    {
        printf("Could not load texture");
    }
    
    assert(GL_NO_ERROR == glGetError());
}

int Update(float dt)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return 0;
}

void Finalize()
{

}
