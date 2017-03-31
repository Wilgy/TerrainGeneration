///
// cgChunk
//
// Routines for tessellating and randomly generating a chunk of squares
//
// @author T. Wilgenbusch
///

#ifndef _CGSHAPE_H_
#define _CGSHAPE_H_

#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#endif

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

// The tesselation factor of each square in the chunk
#define TESS_FACTOR 1

#define NUM_POINTS (TESS_FACTOR + 1) * 2

// The size of chunk (Total number of squares: CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_SIZE 8

#define SAMPLE_SIZE 5

#define MAX_HEIGHT 64.0f
#define MIN_HEIGHT -64.0f

#define MAX_VAR 1.0f
#define MIN_VAR -1.0f

///
// Square - structure containing all the information for an individual square 
//  in the chunk
//
// GLfloat z        - The base z-value of this particular square inside the 
//                    chunk, either chosen when sampling, OR interpolated
// GLfloat x, y     - The normalized x, y coordinates inside of the chunk
// GLfloat points[] - The small variance from the base z-value of each 
//                    tessellated point of this square, either randomly 
//                    generated or interpolated
// int texId        - The id for this squares texture
// bool finished    - bool set when this square is set
///
typedef struct Square_s
{
    GLfloat z;
    GLfloat x, y;
    GLfloat points[NUM_POINTS];
    int texId;
    bool finished;
} Square;

///
// Chunk - structure containing all the information about this chunk
//
// GLfloat chunkX,chunkY- x, y coordinates of this chunk in the world
//  TODO: work terrain generation algorithm to work with multiple chunks
// GLfloat rotate       - Vector for determining how each sqaure in the chunk 
//                        should be rotated (Default is no rotation)
// GLfloat scale        - Vector to determine scaling of chunk (Default is none)
// Square squares[][]   - 2-d array holding all the info for the squares
///
typedef struct  Chunk_s
{
    GLfloat chunkX, chunkY;
    GLfloat rotate[3];
    GLfloat scale[3];
    Square *squares[CHUNK_SIZE][CHUNK_SIZE];

} Chunk;


// Allocates space for a single chunk
Chunk *makeChunk();

// Frees all memory allocated to a chunk
void destroyChunk(Chunk *chunk);

// Populates the float vectors for a single square shape (to be passed to OpenGL)
void makeDefaultSquare();

#endif