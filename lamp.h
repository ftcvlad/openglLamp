#pragma once


#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Lamp
{
public:
	Lamp();
	~Lamp();

	void makeLamp(GLuint numlats, GLuint numlongs);
	void getLampVertexPositions(GLfloat* pVertices, GLfloat* pNormals);
	void drawLamp(int drawmode);

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint lampBufferObject;
	GLuint lampNormals;
	GLuint lampColours;
	GLuint elementbuffer;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;

	int numlampvertices;
	int numlats;
	int numlongs;
	int drawmode;

	int lampAngle;
	float lampHeight;
	float lampTopR;

};