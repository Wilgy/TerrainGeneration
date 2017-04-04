///
// lightingParams.h
//
// @author T. Wilgenbusch (tjw8700)
///

#ifndef _LIGHTINGPARAMS_H_
#define _LIGHTINGPARAMS_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

void setUpPhong( GLuint program );

#endif
