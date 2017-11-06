

#include "line.h"
#include <iostream>
#include <string>

using namespace std;

static map<std::string, Line> allLines;

Line::Line()
{
	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;
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


	//GLfloat* pNormals = new GLfloat[6];
	//pColors[0] = 1;
	//pColors[1] = 0;
	//pColors[2] = 0;
	//pColors[3] = 1;
	//pColors[4] = 0;
	//pColors[5] = 0;
	


	glGenBuffers(1, &lineBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, lineBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 2*3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glGenBuffers(1, &lineNormalsBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, lineNormalsBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 3, pNormals, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &lineColours);
	glBindBuffer(GL_ARRAY_BUFFER, lineColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 2 * 4, pColors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pColors;
	delete pVertices;

	
}




void Line::drawLine(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1, string name, GLuint modelID, glm::mat4 trans)
{

	
	
	if (allLines.find(name) == allLines.end()) {
		Line l = Line();
		cout << z0 << " " << z1 << endl;
		l.makeLine(x0, x1, y0, y1, z0, z1);
		allLines.insert(std::pair<string, Line>(name, l));
	}
		
	Line l = allLines[name];
		
	
	
	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, l.lineBufferObject);
	glVertexAttribPointer(l.attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(l.attribute_v_coord);

	///* Draw the vertices as GL_POINTS */
	//glBindBuffer(GL_ARRAY_BUFFER, l.lineNormalsBufferObject);
	//glVertexAttribPointer(l.attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(l.attribute_v_normal);

	/* Bind the sphere colours */
	glBindBuffer(GL_ARRAY_BUFFER, l.lineColours);
	glVertexAttribPointer(l.attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(l.attribute_v_colours);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	
	glBindBuffer(GL_ARRAY_BUFFER, l.lineBufferObject);

	


		
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(trans[0][0]));
	

	
	glDrawArrays(GL_LINES, 0, 6);



	
}