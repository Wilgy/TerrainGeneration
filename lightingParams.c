///
// lightingParams.c - passes relevant lighting parameters to the shaders
//
// This code can be compiled as either C or C++.
//
// @author: T. Wilgenbusch (tjw8700)
///

#include "lightingParams.h"

// Material properties
GLfloat ambColor[4]  = {0.9, 0.9, 1.0, 1.0};
GLfloat ambRefCoef   = 0.3;

GLfloat difColor[4]  = {1.0, 1.0, 1.0, 1.0};
GLfloat difRefCoef   = 0.8;

GLfloat specColor[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat specExp 	 = 5.0;
GLfloat specRefCoef  = 0.1;

// Light properties
GLfloat lightColor[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat lightPos[4]   = {-10.0, 15.0, 10.0, 1.0};

// Ambient light properties
GLfloat ambLightColor[4] = {1.0, 1.0, 1.0, 1.0};

///
// This function sets up the lighting, material, and shading parameters
// for the Phong shader.
//
// @param program - The ID of an OpenGL (GLSL) shader program to which
// parameter values are to be sent
///
void setUpPhong( GLuint program )
{
	GLuint ambColorLoc = glGetUniformLocation(program, "ambColor");
	GLuint ambRefCoefLoc = glGetUniformLocation(program, "ambRefCoef");

	GLuint difColorLoc = glGetUniformLocation(program, "difColor");
	GLuint difRefCoefLoc = glGetUniformLocation(program, "difRefCoef");
	
	GLuint specColorLoc = glGetUniformLocation(program, "specColor");
	GLuint specExpLoc = glGetUniformLocation(program, "specExp");
	GLuint specRefCoefLoc = glGetUniformLocation(program, "specRefCoef");
	
	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	
	GLuint ambLightColorLoc = glGetUniformLocation(program, "ambLightColor");

	glUniform4fv(ambColorLoc, 1, ambColor);
	glUniform1f(ambRefCoefLoc, ambRefCoef);

	glUniform4fv(difColorLoc, 1, difColor);
	glUniform1f(difRefCoefLoc, difRefCoef);

	glUniform4fv(specColorLoc, 1, specColor);
	glUniform1f(specExpLoc, specExp);
	glUniform1f(specRefCoefLoc, specRefCoef);

	glUniform4fv(lightColorLoc, 1, lightColor);
	glUniform4fv(lightPosLoc, 1, lightPos);

	glUniform4fv(ambLightColorLoc, 1, ambLightColor);
}