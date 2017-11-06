#pragma once




#include "wrapper_glfw.h"
#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>

class Line
{
public:
	Line();
	~Line();

	//void makeLamp(GLuint numlats, GLuint numlongs);
	
	void makeLine(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1);
	static void drawLine(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1, std::string name, GLuint modelID, glm::mat4 trans);

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint lineBufferObject;
	GLuint lineColours;
	GLuint lineNormalsBufferObject;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;
	

	

};