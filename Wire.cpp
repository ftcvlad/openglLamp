

#include "Wire.h"
#include "line.h"
#include <iostream>

using namespace std;

Wire::Wire()
{
	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;

	wireLength = 1;
	wireRadius = 0.02;

}

Wire::~Wire()
{
}

GLfloat Wire::getWireLength() {
	return wireLength;
}

void Wire::getWireVertexPositions(GLfloat* pVertices, GLfloat* pNormals)
{

	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLfloat distanceFromTop = 0;
	GLfloat angleStep = 360 / (numlongs*1.0f);
	GLfloat heightStep = wireLength / (numlats - 1);




	float angle = 0;
	GLuint vertInd = 0;
	for (int i = 0; i < numlats; i++) {
		for (int j = 0; j < numlongs; j++) {
			//positions

			x = cos(angle * DEG_TO_RADIANS) * wireRadius;
			y = distanceFromTop;
			z = sin(angle * DEG_TO_RADIANS) * wireRadius;

			pVertices[vertInd * 3] = x;
			pVertices[vertInd * 3 + 1] = y;
			pVertices[vertInd * 3 + 2] = z;
		
			//normals
			pNormals[vertInd * 3] = x;
			pNormals[vertInd * 3 + 1] = 0;
			pNormals[vertInd * 3 + 2] = z;

			angle += angleStep;
			vertInd++; 
		}

		distanceFromTop -= heightStep;

	}



}

void Wire::makeWire(GLuint numlats, GLuint numlongs)
{
	GLuint i, j;

	numVertices = numlongs * numlats;

	
	this->numlats = numlats;
	this->numlongs = numlongs;


	GLfloat* pVertices = new GLfloat[numVertices * 3];
	GLfloat* pNormals = new GLfloat[numVertices * 3];
	getWireVertexPositions(pVertices, pNormals);

	GLfloat* pColours = new GLfloat[numVertices * 4];
	for (i = 0; i < numVertices; i++) {
		pColours[i * 4] = 0;
		pColours[i * 4 + 1] = 1;
		pColours[i * 4 + 2] = 0;
		pColours[i * 4 + 3] = 1;
	}




	glGenBuffers(1, &wireBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, wireBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numVertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &wireNormals);
	glBindBuffer(GL_ARRAY_BUFFER, wireNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numVertices * 3, pNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &wireColours);
	glBindBuffer(GL_ARRAY_BUFFER, wireColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numVertices * 4, pColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);




	int numindices = (numlongs * 2 +2)*(numlats - 1);
	GLuint* pindices = new GLuint[numindices];


	i = 0;

	int currRow = 0;
	
	for (int g = 0; g < numlats - 1; g++) {
		for (int k = 0;k < numlongs;k++) {
			pindices[i] = currRow*numlongs + k;
			pindices[i + 1] = (currRow + 1)*numlongs + k;
			i = i + 2;
		}
		pindices[i] = currRow*numlongs;
		pindices[i + 1] = (currRow + 1)*numlongs ;
		currRow++;
		i = i + 2;
	}
	




	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pNormals;
	delete pindices;
	delete pColours;
	delete pVertices;

}




void Wire::drawWire(int drawmode)
{



	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, wireBufferObject);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribute_v_coord);

	/* Bind the sphere normals */
	glBindBuffer(GL_ARRAY_BUFFER, wireNormals);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(attribute_v_normal);

	/* Bind the sphere colours */
	glBindBuffer(GL_ARRAY_BUFFER, wireColours);
	glVertexAttribPointer(attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribute_v_colours);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, numVertices);
	}
	else
	{

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);



	
		GLuint lat_offset_current = 0;
		GLuint lat_offset_jump = numlongs * 2 + 2;

		for (int i = 0; i < numlats - 1; i++) {
			glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
			lat_offset_current += (lat_offset_jump * 4);
		}


	}
}

