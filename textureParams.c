///
//  textureParams.c 
//
//  Created by Joe Geigel on 1/23/13.
//
//  This code can be compiled as either C or C++.
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
// loadCubeMapTexture loads a list of textures into Cube Map
//
// @param x_pos_file - the positive x cube face
// @param x_pos_file - the positive x cube face
// @param x_pos_file - the positive x cube face
// @param x_pos_file - the positive x cube face
// @param x_pos_file - the positive x cube face
// @param x_pos_file - the positive x cube face
//
// @return the index into the texture id being used
///
int loadCubeMapTexture(
    const char *x_pos_file,
    const char *x_neg_file,
    const char *y_pos_file,
    const char *y_neg_file,
    const char *z_pos_file,
    const char *z_neg_file)
{
    int texId = SOIL_load_OGL_cubemap
    ( x_pos_file, x_neg_file, y_pos_file, y_neg_file, z_pos_file, z_neg_file,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        0
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
// You will need to write this function, and maintain all of the values
// needed to be sent to the various shaders.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
//    parameter values are to be sent
///
void setUpTexture (GLuint program, int index)
{
    // Set up CubeMap parameters
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

    // Bind the passed in texture to the current active texture and pass the information down
    // to the fragment shader
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureIds[index]);

    GLint textureLoc = glGetUniformLocation(program, "texture");
    glUniform1i(textureLoc, index);
}
