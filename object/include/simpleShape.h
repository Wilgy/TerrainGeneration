///
// simpleShape.h
//
// Header file for functions that manage the current shape 
// being defined (including the addTriangles() function.
//
// @author T. Wilgenbusch
///

#ifndef _SIMPLESHAPE_H_
#define _SIMPLESHAPE_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

void clearShape ();


void addTriangle(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2 );

void addTriangleVertices(    
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2 );

void addTriangleWithNorms(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float nx0, float ny0, float nz0,
    float nx1, float ny1, float nz1,
    float nx2, float ny2, float nz2 );

void addTriangleWithTexCoords (float x0, float y0, float z0, float u0, float v0, 
                  float x1, float y1, float z1, float u1, float v1, 
                  float x2, float y2, float z2, float u2, float v2);


GLushort *getElements ();

float *getVertices ();
float *getNormals ();
float *getUV();

int nVertices ();

#endif
