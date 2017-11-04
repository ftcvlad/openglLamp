#pragma once


#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Bulb
{
public:
	Bulb();
	~Bulb();

	void loadBulb();
	
	void drawBulb(int drawmode, GLuint emitmodeID);

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint bulbBufferObject;
	GLuint bulbNormals;
	GLuint bulbColours;
	GLuint elementbuffer;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;

	int drawmode;
	GLuint numVertices;
	GLuint numPIndexes;

	GLuint beginGlassIndex;
	GLuint endGlassIndex;


};
