//
//  textureParams.h
//  
//
//  Created by Joe Geigel on 1/23/13.
//
//

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

int loadCubeMapTexture(
	const char *x_pos_file,
	const char *x_neg_file,
	const char *y_pos_file,
	const char *y_neg_file,
	const char *z_pos_file,
	const char *z_neg_file);
void setUpTexture (GLuint program, int index);

#endif 
