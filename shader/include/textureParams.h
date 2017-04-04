///
// textureParams.h
//  
// @author T. Wilgenbusch
///

#ifndef _TEXTUREPARAMS_H_
#define _TEXTUREPARAMS_H_

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

int loadTexture (const char *tex_file);

void setUpTexture (GLuint program, int index);

#endif 
