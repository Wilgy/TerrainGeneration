///
//  finalMain
//
//  Main program for final project
//
//  This code can be compiled as either C or C++.
//
// @author T. Wilgenbusch (tjw8700)
///

#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#else
#include <math.h>
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

#include "shaderSetup.h"
#include "simpleShape.h"
#include "cgSquare.h"
#include "textureParams.h"
#include "lightingParams.h"
#include "viewParams.h"
#include "blockInfo.h"

#ifdef __cplusplus
using namespace std;
#endif

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Definition of PI
#define PI 3.14159265358979323846

// The total number of objects in the scene (subject to change)
#define NUM_OBJ NUM_TERRAIN_BLOCKS

bool moving = false;
bool looking = false;
bool animating = false;

float angles[3] = {0.0f, 0.0f, 0.0f};

// Used for updating the camera position an look at
float eyePoint[3] = {-3.0f, 1.0f, 8.0f};
float lookAt[3] = { 6.0f, 1.0f, 8.0f };

// The distance between the camera position and the lookAt position
const float LOOK_RADIUS = 10.0f;
float currentHorizonalRadians = 0.0f;
float currentVerticalRadians = 0.0f;
const float STEP_DISTANCE = 1.0f;

// Speed that the camera is looking around the screen
float mouseSpeed = 0.0005f;

// vertex and element array IDs
// one for each object to be drawn
GLuint buffer[NUM_OBJ];
GLuint ebuffer[NUM_OBJ];
int numVerts[NUM_OBJ];
int NumElements;

// Information for the textures
int grassTexIndex, stoneTexIndex,
    oakPlankTexIndex, oakLogTexIndex, cobbleStoneTexIndex, leavesTexIndex, 
    sheepLegTexIndex, sheepBodyTexIndex, sheepHeadTexIndex;

// program IDs...for program and parameters
GLuint program;

// Directions for the user to move to
enum Direction
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

///
// selectBuffers - sets the current buffers for a given program to a given object
//
// @param program - the program we are setting the buffers for
// @param object - the object we are going to be using
//
///
void selectBuffers(GLuint program,  int object)
{
    //bind buffers
    glBindBuffer( GL_ARRAY_BUFFER , buffer[object] );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , ebuffer[object] );

    //set up the vertex arrays
    int dataSize = numVerts[object] * 4 * sizeof (float);
    int ndataSize = numVerts[object] * 3 * sizeof (float);

    GLuint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition , 4 , GL_FLOAT , GL_FALSE, 0 ,
                           BUFFER_OFFSET(0) );
    
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(dataSize) );

    GLuint vTexCoords = glGetAttribLocation( program , "vTexCoord" );
    glEnableVertexAttribArray( vTexCoords );
    glVertexAttribPointer( vTexCoords , 2 , GL_FLOAT , GL_FALSE, 0 ,
                          BUFFER_OFFSET( dataSize + ndataSize) );

}

///
// createShapes creates all of the shape's (i.e. blocks) vertices/normals and
// passes the information to openGL
///
void createShapes()
{
    // Create all the objects
    for(int i = 0; i < NUM_OBJ; i++)
    {   
        //Clear shape
        clearShape();
            
        //make a shape
        makeDefaultSquare();

        // get the points for your shape
        NumElements = nVertices() / 3;
        float *points = getVertices();
        int dataSize = nVertices() * 4 * sizeof (float);

        // Get normals for object
        float *normals = getNormals();
        int ndataSize = nVertices() * 3 * sizeof(float);

        // Get tex coords for object
        float *texCoords = getUV();
        int tdataSize = nVertices() * 2 * sizeof (float);

        // Get element data for object
        GLushort *elements = getElements();
        int edataSize = nVertices() * sizeof (GLushort);

        //generate the buffer
        glGenBuffers( 1 , &buffer[i] );
        //bind the buffer
        glBindBuffer( GL_ARRAY_BUFFER , buffer[i] );
        //buffer data
        glBufferData( GL_ARRAY_BUFFER, dataSize + tdataSize + ndataSize, 
            0, GL_STATIC_DRAW );
        glBufferSubData ( GL_ARRAY_BUFFER, 0, dataSize, points);
        glBufferSubData ( GL_ARRAY_BUFFER, dataSize, ndataSize, normals);
        glBufferSubData ( GL_ARRAY_BUFFER, dataSize + ndataSize, 
            tdataSize, texCoords);

        //generate the buffer
        glGenBuffers( 1 , &ebuffer[i] );
        //bind the buffer
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , ebuffer[i] );
        //buffer data
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, edataSize,
            elements, GL_STATIC_DRAW );

        //store the num verts
        numVerts[i] = nVertices();
    }
}

///
// changeLook changes the direction that the player is looking
//
// @param horRadianChange - the change (in radians) in the horizontal direction
// @param vertRadianChange - the change (in radians) in the vertical direction
///
void changeLook(float horRadianChange, float vertRadianChange)
{
    currentHorizonalRadians += horRadianChange;
    currentVerticalRadians  += vertRadianChange;
    // Went around the circle
    if(currentHorizonalRadians > 2.0f * PI)
    {
        currentHorizonalRadians -= 2.0f * PI;
    }

    if(currentVerticalRadians > 2.0f * PI)
    {
        currentVerticalRadians -= 2.0f * PI;
    }

    float newX = LOOK_RADIUS * cosf(currentHorizonalRadians) * 
        sinf(currentVerticalRadians) + eyePoint[0];
    float newZ = LOOK_RADIUS * sinf(currentHorizonalRadians) * 
        sinf(currentVerticalRadians) + eyePoint[2];
    float newY = LOOK_RADIUS * cosf(currentVerticalRadians) + eyePoint[1];

    lookAt[0] = newX;
    lookAt[1] = newY;
    lookAt[2] = newZ;
}

///
// moveDirection moves the user into a certain direction
//
// @param direction - the direction the player wants to move
///
static void moveDirection(enum Direction direction)
{
    float deltaX = lookAt[0] - eyePoint[0];
    float deltaZ = lookAt[2] - eyePoint[2];

    float normalizeFactor = powf((powf(deltaX, 2.0)+powf(deltaZ, 2.0f)), 0.5f);

    deltaX = deltaX/normalizeFactor;
    deltaZ = deltaZ/normalizeFactor;

    if(direction == FORWARD)
    {
        // Move in the direction of norm
        deltaX = deltaX * STEP_DISTANCE;
        deltaZ = deltaZ * STEP_DISTANCE;

        float newX = eyePoint[0] + deltaX;
        float newZ = eyePoint[2] + deltaZ;

        eyePoint[0] = newX;
        eyePoint[2] = newZ;

    }
    else if(direction == BACKWARD)
    {
        // Move in opposite of factor
        deltaX = -deltaX * STEP_DISTANCE;
        deltaZ = -deltaZ * STEP_DISTANCE;

        float newX = eyePoint[0] + deltaX;
        float newZ = eyePoint[2] + deltaZ;

        eyePoint[0] = newX;
        eyePoint[2] = newZ;
    }
    else if(direction == LEFT)
    {
        // Move in perpendicular of factor
        deltaX = -deltaX * STEP_DISTANCE;
        deltaZ = deltaZ * STEP_DISTANCE;

        float newX = eyePoint[0] + deltaZ;
        float newZ = eyePoint[2] + deltaX;

        eyePoint[0] = newX;
        eyePoint[2] = newZ;

    }
    else // Moving RIGHT
    {
        // Move in perpendicular of negative factor
        deltaX = deltaX * STEP_DISTANCE;
        deltaZ = -deltaZ * STEP_DISTANCE;

        float newX = eyePoint[0] + deltaZ;
        float newZ = eyePoint[2] + deltaX;

        eyePoint[0] = newX;
        eyePoint[2] = newZ;
    }

    // update where we are looking at as well
    changeLook(0.0f, 0.0f);
}

///
// init OpenGL initialization sets up all the information to draw the scene
///
void init() {
    
    // Load shaders and use the resulting shader program
    program = shaderSetup( "cube.vert", "cube.frag" );
    if (!program) {
#ifdef __cplusplus
        cerr << "Error setting up shaders - "
	     << errorString(shaderErrorCode) << endl;
#else
        fprintf( stderr, "Error setting up shaders - %s\n",
	    errorString(shaderErrorCode) );
#endif
        exit(1);
    }
    
    // Some openGL initialization...probably best to keep
    glEnable( GL_DEPTH_TEST );

    // Disable culling of back face while using 2-d squares 
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glClearColor( 0.2, 0.5, 1.0, 0.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // Disable cursor
    glutSetCursor(GLUT_CURSOR_NONE); 

    // Creates wire frame view
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // create the geometry for your shapes.
    createShapes();

    // Fill in correct block transformation information
    fillBlockTransformInfo();

    // set default look position of the camera (looking directly at the scene)
    changeLook(0.0f, PI/2.0f);

    // load textures
    grassTexIndex = loadCubeMapTexture("dirt-grass-left.png", 
        "dirt-grass-right.png", "dirt-grass-down.png", 
        "dirt-grass.png", "dirt-grass-top.png", "dirt-plain.png" );
    stoneTexIndex = loadCubeMapTexture("stone.png", "stone.png", 
        "stone.png", "stone.png", "stone.png", "stone.png");
}


#ifdef __cplusplus
extern "C" {
#endif

///
// display redraws all of the objects in the scene
///
void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //use program
    glUseProgram( program );

    // set up viewing and projection parameters
    setUpFrustum(program);

    // Set up lighting variables
    setUpPhong(program);

    // Set the default camera position
    // set up the camera
    setUpCamera( program,
        eyePoint[0], eyePoint[1], eyePoint[2],
        lookAt[0], lookAt[1], lookAt[2],
        0.0f, 1.0f, 0.0f
    );

    // Set up and draw all of the objects
    int i;
    GLfloat *scale;
    GLfloat *rotate;
    GLfloat *translate;

    // Display terrain blocks
    for(i = 0; i < NUM_TERRAIN_BLOCKS; i++)
    {            
        clearTransforms(program);
        scale = getTerrainScaleInfo(i);
        rotate = getTerrainRotateInfo(i);
        translate = getTerrainTranslateInfo(i);
        setUpTexture(program, grassTexIndex);            

        // set up transformations 
        setUpTransforms( program,
            scale[0], scale[1], scale[2],
            rotate[0] + angles[0], rotate[1] + angles[1], rotate[2] + angles[2],
            translate[0], translate[1], translate[2]
        );

        //setup uniform variables to shader
        selectBuffers(program, i);
        // draw your shape
        glDrawElements(GL_TRIANGLES ,numVerts[i],GL_UNSIGNED_SHORT, (void *)0 );
    }

    // swap the buffers
    glutSwapBuffers();
}

///
// animate called whenever the scene needs to be redrawn
///
void animate( void ) 
{
    int xOrigin = (glutGet(GLUT_WINDOW_WIDTH))/2;
    int yOrigin = (glutGet(GLUT_WINDOW_HEIGHT))/2;

    if(moving)
    {
        moving = false;
    }

    if(looking)
    {
        looking = false;
        glutWarpPointer(xOrigin, yOrigin);
    }

    glutPostRedisplay();
}

///
// passiveMotion - allows the camera to look around using the mouse
//
// @param x - the current x coord of the mouse on the screen
// @param y - the current y coord of the mouse on the screen
///
void passiveMotion(int x, int y)
{
    int xOrigin = (glutGet(GLUT_WINDOW_WIDTH))/2;
    int yOrigin = (glutGet(GLUT_WINDOW_HEIGHT))/2;

    float horRadianChange = (x - xOrigin) * mouseSpeed;
    float vertRadianChange = (y - yOrigin) * mouseSpeed;

    currentHorizonalRadians += horRadianChange;
    currentVerticalRadians  += vertRadianChange;

    float newX = LOOK_RADIUS * cosf(currentHorizonalRadians) * 
        sinf(currentVerticalRadians) + eyePoint[0];
    float newZ = LOOK_RADIUS * sinf(currentHorizonalRadians) * 
        sinf(currentVerticalRadians) + eyePoint[2];
    float newY = LOOK_RADIUS * cosf(currentVerticalRadians) + eyePoint[1];

    lookAt[0] = newX;
    lookAt[1] = newY;
    lookAt[2] = newZ;

    looking = true;
}

///
// keyboard function called when any input is received from the user
//
// @param key - the key pressed by the user
// @param x - the x coordinate of the mouse
// @param y - the y coordinate of the mouse
///
void keyboard( unsigned char key, int x, int y )
{
    switch( key ) 
    {
        // Moving the camera around the scene
        case 'w': // move up
            moveDirection(FORWARD);
            moving = true;
            break;
        case 'a': // move left
            moveDirection(LEFT);
            moving = true;
            break;
        case 's': // move down
            moveDirection(BACKWARD);
            moving = true;
            break;
        case 'd': // move right
            moveDirection(RIGHT);
            moving = true;
            break;

        // Looking in different directions
        case '8': // look up
            changeLook(0.0, -0.1);
            looking = true;
            break;
        case '4': // look left
            changeLook(-0.1, 0.0);
            looking = true;
            break;
        case '5': // look down
            changeLook(0.0, 0.1);
            looking = true;
            break;
        case '6': // look right
            changeLook(0.1, 0.0);
            looking = true;
            break;

        // PARTY TIME!!!
        case 'x':
            angles[0] += 1;
            moving = true;
            break;
        case 'y':
            angles[1] += 1;
            moving = true;
            break;
        case 'z':
            angles[2] += 1;
            moving = true;
            break;
        case 'X':
            angles[0] -= 1;
            moving = true;
            break;
        case 'Y':
            angles[1] -= 1;
            moving = true;
            break;
        case 'Z':
            angles[2] -= 1;
            moving = true;
            break;

        // Exit the program
        case 033:  // Escape key
        case 'q': case 'Q':
            exit( 0 );
        break;
    } 
}

#ifdef __cplusplus
}
#endif

///
// main function entry point of the program; initializes all of the openGL 
// information and starts drawing the scene
//
// @param argc - number of command line arguments; unused
// @param argv - command line args; unused
//
// @return 0 on successful execution
///
int main (int argc, char **argv)
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "CG - Final Project" );
    
#ifndef __APPLE__
    glewInit();
#endif
    
    init();
    
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( animate );
    glutMotionFunc( passiveMotion );
    glutPassiveMotionFunc( passiveMotion );
    glutMainLoop();
    return 0;
}
