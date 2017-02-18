///
// simpleShape.c
//
// Routines for adding triangles to create a new mesh
//
// @author T. Wilgenbusch
///

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "simpleShape.h"
#include "floatVector.h"

///
// common variables...should probably make this a class and make these
// data members
///
floatVector_t points;
floatVector_t normals;
floatVector_t uv;
float *pointArray = 0;
float *normalArray = 0;
float *uvArray = 0;
GLushort *elemArray = 0;


///
// clear the current shape
///
void clearShape ()
{
    if (pointArray) {
        free( pointArray );
        free( elemArray );
        free( normalArray );
        free( uvArray );
        pointArray = 0;
        elemArray = 0;
        normalArray = 0;
        uvArray = 0;
    }
    floatVectorClear( &points );
    floatVectorClear( &normals );
    floatVectorClear( &uv );
}


///
// adds a triangle to the current shape
///
void addTriangleWithTexCoords(float x0, float y0, float z0, float u0, float v0, 
                  float x1, float y1, float z1, float u1, float v1, 
                  float x2, float y2, float z2, float u2, float v2)
{

    floatVectorPushBack( &points, x0 );
    floatVectorPushBack( &points, y0 );
    floatVectorPushBack( &points, z0 );
    floatVectorPushBack( &points, 1.0 );
    
    floatVectorPushBack( &points, x1 );
    floatVectorPushBack( &points, y1 );
    floatVectorPushBack( &points, z1 );
    floatVectorPushBack( &points, 1.0 );
    
    floatVectorPushBack( &points, x2 );
    floatVectorPushBack( &points, y2 );
    floatVectorPushBack( &points, z2 );
    floatVectorPushBack( &points, 1.0 );
    
    // calculate the normal
    float ux = x1 - x0;
    float uy = y1 - y0;
    float uz = z1 - z0;
    
    float vx = x2 - x0;
    float vy = y2 - y0;
    float vz = z2 - z0;
    
    float nx = (uy * vz) - (uz * vy);
    float ny = (uz * vx) - (ux * vz);
    float nz = (ux * vy) - (uy * vx);

    for(int i = 0; i < 3; i++)
    {
        floatVectorPushBack( &normals, nx );
        floatVectorPushBack( &normals, ny );
        floatVectorPushBack( &normals, nz );        
    }    

    // Attach the normal to all 3 vertices
    // NOTE: This was updated to give the normals for a SPHERE;
    // will most likely not work for any other primitive shape
    // floatVectorPushBack( &normals, x0 );
    // floatVectorPushBack( &normals, y0 );
    // floatVectorPushBack( &normals, z0);

    // floatVectorPushBack( &normals, x1 );
    // floatVectorPushBack( &normals, y1 );
    // floatVectorPushBack( &normals, z1 );

    // floatVectorPushBack( &normals, x2 );
    // floatVectorPushBack( &normals, y2 );
    // floatVectorPushBack( &normals, z2 );

    
    // Attach the texture coords
    floatVectorPushBack( &uv, u0 );
    floatVectorPushBack( &uv, v0 );
    floatVectorPushBack( &uv, u1 );
    floatVectorPushBack( &uv, v1 );
    floatVectorPushBack( &uv, u2 );
    floatVectorPushBack( &uv, v2 );
    
}

///
// adds a triangle to the current shape using calculated normals
///
void addTriangle(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2 )
{
    // calculate the normal
    float ux = x1 - x0;
    float uy = y1 - y0;
    float uz = z1 - z0;

    float vx = x2 - x0;
    float vy = y2 - y0;
    float vz = z2 - z0;

    float nx = (uy * vz) - (uz * vy);
    float ny = (uz * vx) - (ux * vz);
    float nz = (ux * vy) - (uy * vx);

    // Attach the normal to all 3 vertices
    addTriangleWithNorms(
        x0, y0, z0, x1, y1, z1, x2, y2, z2,
        nx, ny, nz, nx, ny, nz, nx, ny, nz
    );
}

///
// adds a triangle to the current shape using supplied normals
///
void addTriangleWithNorms(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float nx0, float ny0, float nz0,
    float nx1, float ny1, float nz1,
    float nx2, float ny2, float nz2 )
{
    floatVectorPushBack( &points, x0 );
    floatVectorPushBack( &points, y0 );
    floatVectorPushBack( &points, z0 );
    floatVectorPushBack( &points, 1.0 );

    floatVectorPushBack( &normals, nx0 );
    floatVectorPushBack( &normals, ny0 );
    floatVectorPushBack( &normals, nz0 );

    floatVectorPushBack( &points, x1 );
    floatVectorPushBack( &points, y1 );
    floatVectorPushBack( &points, z1 );
    floatVectorPushBack( &points, 1.0 );

    floatVectorPushBack( &normals, nx1 );
    floatVectorPushBack( &normals, ny1 );
    floatVectorPushBack( &normals, nz1 );

    floatVectorPushBack( &points, x2 );
    floatVectorPushBack( &points, y2 );
    floatVectorPushBack( &points, z2 );
    floatVectorPushBack( &points, 1.0 );

    floatVectorPushBack( &normals, nx2 );
    floatVectorPushBack( &normals, ny2 );
    floatVectorPushBack( &normals, nz2 );

}

///
// adds only a triangle's vertices to object
///
void addTriangleVertices(    
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2 )
{
    floatVectorPushBack( &points, x0 );
    floatVectorPushBack( &points, y0 );
    floatVectorPushBack( &points, z0 );
    floatVectorPushBack( &points, 1.0 );

    floatVectorPushBack( &points, x1 );
    floatVectorPushBack( &points, y1 );
    floatVectorPushBack( &points, z1 );
    floatVectorPushBack( &points, 1.0 );

    floatVectorPushBack( &points, x2 );
    floatVectorPushBack( &points, y2 );
    floatVectorPushBack( &points, z2 );
    floatVectorPushBack( &points, 1.0 );
}

///
// gets the vertex points for the current shape
///
float *getVertices ()
{
    int i;

    // delete the old point array of we have one
    if (pointArray) {
        free( pointArray );
    }
    
    // create and fill a new point array
    pointArray = (float *) malloc(
        floatVectorSize(&points) * sizeof(float) );
    if( pointArray == 0 ) {
        perror( "point allocation failed" );
	exit( 1 );
    }
    for (i=0; i < points.size; i++) {
        pointArray[i] = points.vec[i];
    }
        
    return pointArray;
}

///
// gets the normals for the current shape
///
float *getNormals ()
{
    int i;

    // delete the old normal array if we have one
    if (normalArray) {
        free( normalArray );
    }
    
    // create and fill a new normal array
    normalArray = (float *) malloc(
        floatVectorSize(&normals) * sizeof(float) );
    if( normalArray == 0 ) {
        perror( "normal allocation failed" );
	exit( 1 );
    }
    for (i=0; i < normals.size; i++) {
        normalArray[i] = normals.vec[i];
    }
    
    return normalArray;
}

///
// gets the texture coords for the current shape
///
float *getUV ()
{
    int i;

    // delete the old uv array if we have one
    if (uvArray) {
        free( uvArray );
    }
    
    // create and fill a new uv array
    uvArray = (float *) malloc(
        floatVectorSize(&uv) * sizeof(float) );
    if( uvArray == 0 ) {
        perror( "uv allocation failed" );
	exit( 1 );
    }
    for (i=0; i < uv.size; i++) {
        uvArray[i] = uv.vec[i];
    }
    
    return uvArray;
}

///
// gets the  array of elements for the  current shape
///
GLushort *getElements ()
{
    int i;

    // delete the old point array of we have one
    if (elemArray) {
        free( elemArray );
    }
    
    // create and fill a new point array
    elemArray = (GLushort *) malloc(
        floatVectorSize(&points) * sizeof(GLushort) );
    if( elemArray == 0 ) {
        perror( "element allocation failed" );
	exit( 1 );
    }
    for (i=0; i < points.size; i++) {
        elemArray[i] = i;
    }
    
    return elemArray;
}

///
// returns number of vertices in current shape
///
int nVertices ()
{
    return floatVectorSize(&points) / 4;
}
