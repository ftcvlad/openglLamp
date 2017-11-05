#pragma once


#include "wrapper_glfw.h"
#include <glm/glm.hpp>

class Wire
{
public:
	Wire();
	~Wire();

	void Wire::getWireVertexPositions(GLfloat* pVertices, GLfloat* pNormals);
	void makeWire(GLuint numlats, GLuint numlongs);

	void drawWire(int drawmode);
	GLfloat getWireLength();

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint wireBufferObject;
	GLuint wireNormals;
	GLuint wireColours;
	GLuint elementbuffer;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;

	int drawmode;
	GLuint numVertices;

	GLfloat wireLength;
	GLfloat wireRadius;

	GLuint numlats;
	GLuint numlongs;
};
