///
// textureParams.c - sets up texture parameters for simple 2D objects
//
// This code can be compiled as either C or C++.
// @author T. Wilgenbusch
///

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <SOIL.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SOIL.h>
#endif

#include <stdio.h>
#include "textureParams.h"

// Generic max, may need to add to later
#define MAX_TEXTURES 20

// Global for holding all the texture id
GLuint textureIds[MAX_TEXTURES];
int currentIndex = 0;

///
// loadTexture loads a single image as a texture to use
//
// @param tex_file - the image file to be used as a texture
//
// @return the integer id of the texture loaded
//
int loadTexture(const char *tex_file)
{
    int texId = SOIL_load_OGL_texture
    (
        tex_file,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS
    );

    // check for an error during the load process
    if ( texId == 0 ) 
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }

    // Save id in the texture array and return index into the array
    int index = currentIndex;
    textureIds[index] = texId;
    currentIndex += 1;
    return index;
}

///
// This function sets up the parameters for texture use.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
// @param index - The index of the relevant texture to set up and bind
///
void setUpTexture(GLuint program, int index)
{    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );

    // Bind the passed in texture to the current active texture and pass the 
    // information down to the fragment shader
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, textureIds[index]);

    GLint textureLoc = glGetUniformLocation(program, "texture");
    glUniform1i(textureLoc, index);
}
