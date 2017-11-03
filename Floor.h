#pragma once
#pragma once


#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Floor
{
public:
	Floor();
	~Floor();

	void makeFloor();
	void drawFloor(int drawmode);

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint floorBufferObject;
	GLuint floorNormals;
	GLuint floorColours;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;
	GLuint nOfVertices;
	int drawmode;


};