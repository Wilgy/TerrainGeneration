///
// blockInfo.c - holds all the information for transform/manipulating all of the 
// blocks in the scene
//
// @author T. Wilgenbusch (tjw8700)
///

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

#include "blockInfo.h"

// Transformation information for each terrain block
GLfloat terrainRotate[3] = {-90.0f, 0.0f, 0.0f};
GLfloat terrainScale[3] = {1.0f, 1.0f, 1.0f};
GLfloat terrainTranslate[NUM_TERRAIN_BLOCKS][3];

///
// getTerrainRotateInfo grabs the rotation info for a single terrain object
//
// @param index index into the matrix
//
// @return the rotation information
///
GLfloat *getTerrainRotateInfo(int index)
{   
    return terrainRotate;
}

///
// getTerrainScaleInfo grabs the scaling info for a single terrain object
//
// @param index index into the matrix
//
// @return the scaling information
///
GLfloat *getTerrainScaleInfo(int index)
{
    return terrainScale;
}

///
// getTerrainTranslateInfo grabs the translation info for a single terrain object
//
// @param index index into the matrix
//
// @return the translation information
///
GLfloat *getTerrainTranslateInfo(int index)
{
    return terrainTranslate[index];
}

///
// fillTerrainTransformInfo fills in the transformation information for 
// each individual terrain block
///
static void fillTerrainTransformInfo()
{
    for(int i = 0; i < TERRAIN_SIDE_LENGTH; i++)
    {
        for(int j = 0; j < TERRAIN_SIDE_LENGTH; j++)
        {

            terrainTranslate[TERRAIN_SIDE_LENGTH * i + j][0] =  j;
            terrainTranslate[TERRAIN_SIDE_LENGTH * i + j][1] =  -1.0f;
            terrainTranslate[TERRAIN_SIDE_LENGTH * i + j][2] =  i;
        }
    }
}

///
// fillBlockTransformInfo fills in all of the transformation information, 
// primarly the translation values, for all the blocks in the scene
///
void fillBlockTransformInfo()
{
    fillTerrainTransformInfo();
}
