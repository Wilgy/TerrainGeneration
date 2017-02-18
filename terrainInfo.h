///
// terrainInfo.h
//
// @author T. Wilgenbusch (tjw8700)
///
#ifndef _TERRAININFO_H_
#define _TERRAININFO_H_

// The total number of terrain blocks (i.e. the ground)
#define TERRAIN_SIDE_LENGTH 16
#define NUM_TERRAIN_BLOCKS (TERRAIN_SIDE_LENGTH*TERRAIN_SIDE_LENGTH)

void fillTransformInfo();

GLfloat *getTerrainRotateInfo(int index);
GLfloat *getTerrainScaleInfo(int index);
GLfloat *getTerrainTranslateInfo(int index);

#endif
