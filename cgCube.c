// ///
// // cgShape
// //
// // Routines for tessellating a number of basic shapes
// //
// // This code can be compiled as either C or C++.
// //
// // @author T. Wilgenbusch
// ///

// #ifdef __APPLE__
// #include <OpenGL/gl.h>
// #else
// #include <GL/glew.h>
// #include <GL/gl.h>
// #endif

// #include "simpleShape.h"

// #ifdef __cplusplus
// #include <cmath>
// #else
// #include <math.h>
// #endif

// #include <stdio.h>

// #define DEFAULT_TESS 2

// // The maximum values for the unit length objects being tessalated
// #define UNIT_WIDTH 1.0f
// #define UNIT_MAX UNIT_WIDTH/2.0f
// #define UNIT_MIN -UNIT_MAX

// // The dimensions for points and vectors being defined
// #define V_DIM 3
// #define TOTAL_POINTS 4

// // Definition of PI
// #define PI 3.14159265358979323846

// ///
// // getPointsFromCenter - given a point on the TOP face of a cube, calculates all
// // of the points from that bottom left point
// //
// // @param cp - the point at the bottom left (4 in the diagram below)
// // @param sl - the length between each point
// // @param allPx - the array used to store the resulting points x values; stored 
// //        in the order described in the diagram below
// // @param allPy - the array used to store the resulting points y values; stored 
// //        in the order described in the diagram below
// // @param allPz - the array used to store the resulting points z values; stored 
// //        in the order described in the diagram below
// // 1 --- 2
// // |     |
// // |     |
// // 3 --- 4
// ///
// void getPointsFromCenter(float cp[V_DIM], float sl, float allPx[TOTAL_POINTS], 
//     float allPy[TOTAL_POINTS], float allPz[TOTAL_POINTS])
// {
//     allPx[0] = cp[0] - sl; allPy[0] = cp[1] + sl; allPz[0] = cp[2]; //Top-Left  
//     allPx[1] = cp[0];      allPy[1] = cp[1] + sl; allPz[1] = cp[2]; //Top-Right
//     allPx[2] = cp[0] - sl; allPy[2] = cp[1];      allPz[2] = cp[2]; //Bot-Right
//     allPx[3] = cp[0];      allPy[3] = cp[1];      allPz[3] = cp[2]; //Bot-Left
// }


// ///
// // addSubDivision - given a list of points generated from a starting point, 
// // draws all the triangle subdivisions of those points;
// //
// // @param allPx - the x coordinates
// // @param allPy - the y coordinates
// // @param allPz - the z coordinates
// //
// // With the below diagram, designates the following triangles:
// //  (1, 3, 4), (1, 4, 2)
// // 1 --- 2
// // |     |
// // |     |
// // 3 --- 4
// ///
// void addSubDivision(float allPx[TOTAL_POINTS], float allPy[TOTAL_POINTS], 
//     float allPz[TOTAL_POINTS])
// {
//     addTriangle(
//         allPx[0], allPy[0], allPz[0],
//         allPx[2], allPy[2], allPz[2],
//         allPx[3], allPy[3], allPz[3]);
//     addTriangle(
//         allPx[0], allPy[0], allPz[0],
//         allPx[3], allPy[3], allPz[3], 
//         allPx[1], allPy[1], allPz[1]);
// }

// ///
// // addAllCubeSides - given a bottom left point of a sub square, draws the 
// // tessellated square for all sides of the cube
// // 
// // @param cp - the bottom left point of the sub square
// // @param sideLength - the length of the sub square's side
// ///
// void addAllCubeSides(float cp[V_DIM], float sideLength)
// {
//     float allPx[TOTAL_POINTS] = { 0.0f };
//     float allPy[TOTAL_POINTS] = { 0.0f };
//     float allPz[TOTAL_POINTS] = { 0.0f };
//     getPointsFromCenter(cp, sideLength, allPx, allPy, allPz);

//     // Negative values for other sides of the cube
//     float negAllPx[TOTAL_POINTS] = { 0.0f };
//     float negAllPy[TOTAL_POINTS] = { 0.0f };
//     float negAllPz[TOTAL_POINTS] = { 0.0f };
//     for(int k = 0; k < TOTAL_POINTS; k++)
//     {
//         // To ensure that we don't get the -0.0
//         negAllPx[k] = allPx[k] == 0.0f ? 0.0f : -allPx[k];
//         negAllPy[k] = allPy[k] == 0.0f ? 0.0f : -allPy[k];
//         negAllPz[k] = allPz[k] == 0.0f ? 0.0f : -allPz[k];
//     }

//     // Draw the TOP Side
//     addSubDivision(allPx, allPy, allPz);
//     // Draw BOTTOM Side
//     addSubDivision(allPx, negAllPy, negAllPz);
//     // Draw FRONT Side
//     addSubDivision(allPx, negAllPz, allPy);
//     // Draw BACK Side
//     addSubDivision(allPx, allPz, negAllPy);
//     // Draw RIGHT Side
//     addSubDivision(allPz, negAllPx, negAllPy);
//     // Draw LEFT Side
//     addSubDivision(negAllPz, negAllPx, allPy);
// }

// ///
// // makeCube - Create a unit cube, centered at the origin, with a given number
// // of subdivisions in each direction on each face.
// //
// // @param subdivision - number of equal subdivisons to be made in each
// //        direction along each face
// //
// // Can only use calls to addTriangle()
// ///
// void makeCube (int subdivisions)
// {
//     if( subdivisions < 1 )
//         subdivisions = 1;

//     // int num_squares = pow(2, subdivisions - 1); // the num squares per row/col
//     // The TOP side of the cube
//     float p1[V_DIM] = {UNIT_MIN, UNIT_MAX, UNIT_MAX};
//     float p2[V_DIM] = {UNIT_MAX, UNIT_MAX, UNIT_MAX};
//     float p3[V_DIM] = {UNIT_MIN, UNIT_MIN, UNIT_MAX};
//     float p4[V_DIM] = {UNIT_MAX, UNIT_MIN, UNIT_MAX};

//     float side_length = UNIT_WIDTH/(float)subdivisions;

//     float q[V_DIM] = { 0.0f };
//     float r[V_DIM] = { 0.0f };
//     float pd[V_DIM] = { 0.0f };

//     for( int i = 1; i <= subdivisions; i++)
//     {
//         float u = i*side_length;

//         // The top point
//         q[0] = (UNIT_WIDTH - u)*p1[0] + u*p2[0];
//         q[1] = (UNIT_WIDTH - u)*p1[1] + u*p2[1];
//         q[2] = (UNIT_WIDTH - u)*p1[2] + u*p2[2];

//         // The bottom point; We don't need to define this since we are 
//         // using a unit square, but it helps for clarity;
//         r[0] = (UNIT_WIDTH - u)*p3[0] + u*p4[0];
//         r[1] = (UNIT_WIDTH - u)*p3[1] + u*p4[1];
//         r[2] = (UNIT_WIDTH - u)*p3[2] + u*p4[2];

//         for(int j = 1; j <= subdivisions; j++)
//         {
//             float v = j*side_length;

//             pd[0] = (UNIT_WIDTH - v)*q[0] + v*r[0];
//             pd[1] = (UNIT_WIDTH - v)*q[1] + v*r[1]; 
//             pd[2] = (UNIT_WIDTH - v)*q[2] + v*r[2];

//             // pd is the bottom left corner of the square being drawn
//             addAllCubeSides(pd, side_length);
//         }
//     }
// }

// ///
// // makeDefaultSphere - creates a "unit" cube
// ///
// void makeDefaultCube()
// {
//     makeCube(DEFAULT_TESS);
// }

