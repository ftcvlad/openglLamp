/* sphere.h
 Example class to create a generic sphere object
 Resolution can be controlled by settign nuber of latitudes and longitudes
 Iain Martin November 2016
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Sphere
{
public:
	Sphere();
	~Sphere();

	void makeSphere(GLuint numlats, GLuint numlongs);
	void drawSphere(int drawmode);

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint sphereBufferObject;
	GLuint sphereNormals;
	GLuint sphereColours;
	GLuint elementbuffer;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;

	int numspherevertices;
	int numlats;
	int numlongs;
	int drawmode;

	void makeUnitSphere(GLfloat *pVertices);
};
