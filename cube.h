/* cube.h
Example class to to show a tetrahedon implementation
Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Cube
{
public:
	Cube();
	~Cube();

	void makeCube();
	void drawCube(int drawmode);

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint positionBufferObject;
	GLuint colourObject;
	GLuint normalsBufferObject;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;

	int numvertices;
	int drawmode;
};
