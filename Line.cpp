

#include "line.h"
#include <iostream>
#include <string>
using namespace std;

static map<std::string, Line> allLines;

Line::Line()
{
	attribute_v_coord_line = 0;
	attribute_v_colours_line = 1;
}

Line::~Line()
{
}


void Line::makeLine(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1)
{
	
	GLfloat* pVertices = new GLfloat[6];
	pVertices[0] = x0;
	pVertices[1] = y0;
	pVertices[2] = z0;
	pVertices[3] = x1;
	pVertices[4] = y1;
	pVertices[5] = z1;
	
	GLfloat* pColors = new GLfloat[8];
	pColors[0] = 1.f;
	pColors[1] = 1.0f;
	pColors[2] = 0;
	pColors[3] = 1.f;
	pColors[4] = 0.f;
	pColors[5] = 1.0f;
	pColors[6] = 0;
	pColors[7] = 1.f;

	glGenBuffers(1, &lineBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, lineBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 2*3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &lineColours);
	glBindBuffer(GL_ARRAY_BUFFER, lineColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 2 * 4, pColors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pColors;
	delete pVertices;
}




void Line::drawLine(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1, string name)
{

	
	
	if (allLines.find(name) == allLines.end()) {
		Line l = Line();
		l.makeLine(x0, x1, y0, y1, z0, z1);
		allLines.insert(std::pair<string, Line>(name, l));
	}
		
	Line l = allLines[name];
		
	
	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, l.lineBufferObject);
	glVertexAttribPointer(l.attribute_v_coord_line, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(l.attribute_v_coord_line);


	/* Bind the sphere colours */
	glBindBuffer(GL_ARRAY_BUFFER, l.lineColours);
	glVertexAttribPointer(l.attribute_v_colours_line, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(l.attribute_v_colours_line);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	


	glBindBuffer(GL_ARRAY_BUFFER, l.lineBufferObject);

	glDrawArrays(GL_LINES, 0, 6);



	
}