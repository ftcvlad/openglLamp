

#include "lamp.h"
#include "line.h"
#include <iostream>

using namespace std;

Lamp::Lamp()
{
	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;
	//numspherevertices = 0;		
	//this->drawmode = drawmode;
	lampTopR = 0.3f;
	lampAngle = 35;
	lampHeight = 1.0f;
}

Lamp::~Lamp()
{
}


void Lamp::getLampVertexPositions(GLfloat* pVertices, GLfloat* pNormals, GLfloat* pColours)
{
	GLfloat currentRadius = lampTopR;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLint vertInd = 1;
	GLfloat distanceFromTop = 0;
	GLfloat angleStep = 360 / (numlongs*1.0f);
	GLfloat heightStep = lampHeight / (numlats - 1);
	GLfloat distanceBetweenSurfaces = 0.01;

	GLfloat bottomRadius = lampTopR + lampHeight * tan(lampAngle * DEG_TO_RADIANS);
	GLfloat coneHeight = bottomRadius / tan(lampAngle * DEG_TO_RADIANS);


	//TOP VERTICES
	//top center vertex
	pVertices[0] = 0;
	pVertices[1] = 0;
	pVertices[2] = 0;
	pNormals[0] = 0;
	pNormals[1] = 1;
	pNormals[2] = 0;
	pColours[0] = 0.5;
	pColours[1] = 0.5;
	pColours[2] = 0.5;
	pColours[3] = 1;



	//top circumference (vertices later repeated)
	float angle = 0;
	for (int j = 0; j < numlongs; j++) {

		//positions
		pVertices[vertInd * 3] = cos(angle * DEG_TO_RADIANS) * lampTopR;
		pVertices[vertInd * 3 + 1] = distanceFromTop;
		pVertices[vertInd * 3 + 2] = sin(angle * DEG_TO_RADIANS) * lampTopR;

		//normals
		pNormals[vertInd * 3] = 0;
		pNormals[vertInd * 3 + 1] = 1;
		pNormals[vertInd * 3 + 2] = 0;

		//colours
		pColours[vertInd * 4] = 0.5;//top grey
		pColours[vertInd * 4 + 1] = 0.5;
		pColours[vertInd * 4 + 2] = 0.5;
		pColours[vertInd * 4 + 3] = 1.f;


		angle += angleStep;
		vertInd++;
	}

	GLuint innerSurfaceOffset = numlats * numlongs;
	//SIDE VERTICES
	for (int i = 0; i<numlats; i++) {
		
		angle = 0;

		for (int j = 0; j < numlongs; j++) {
			
			//OUTER SURFACE
			//positions
			x = cos(angle * DEG_TO_RADIANS) * currentRadius;
			y = distanceFromTop;
			z = sin(angle * DEG_TO_RADIANS) * currentRadius;
			pVertices[vertInd*3] = x;
			pVertices[vertInd*3 + 1] = y;
			pVertices[vertInd*3 + 2] = z;

			//normals
		
			GLfloat m = sqrt(x*x + z*z);
			GLfloat vx = x / m;
			GLfloat vz = z / m;
			GLfloat normalX = vx*coneHeight / bottomRadius;
			GLfloat normalY = bottomRadius / coneHeight;
			GLfloat normalZ = vz*coneHeight / bottomRadius;

			pNormals[vertInd * 3] = normalX;
			pNormals[vertInd * 3 + 1] = normalY;
			pNormals[vertInd * 3 + 2] = normalZ;

			//colours
			pColours[vertInd * 4] = 0;//outer blue
			pColours[vertInd * 4 + 1] = 0;
			pColours[vertInd * 4 + 2] = 1;
			pColours[vertInd * 4 + 3] = 1.f;

			
			
			//INNER SURFACE
			x = cos(angle * DEG_TO_RADIANS) * (currentRadius - distanceBetweenSurfaces);
			y = distanceFromTop;
			z = sin(angle * DEG_TO_RADIANS) * (currentRadius - distanceBetweenSurfaces);
			GLuint vertIndex = (innerSurfaceOffset + vertInd) * 3;
			pVertices[vertIndex] = x;
			pVertices[vertIndex + 1] = y;
			pVertices[vertIndex + 2] = z;
			pNormals[vertIndex] = -normalX;
			pNormals[vertIndex + 1] = -normalY;
			pNormals[vertIndex + 2] = -normalZ;

			pColours[(innerSurfaceOffset + vertInd) * 4] = 1;//inner red
			pColours[(innerSurfaceOffset + vertInd) * 4 + 1] = 0;
			pColours[(innerSurfaceOffset + vertInd) * 4 + 2] = 0;
			pColours[(innerSurfaceOffset + vertInd) * 4 + 3] = 1.f;
			

			
			angle += angleStep;
			vertInd++;
		}

		distanceFromTop -= heightStep;
		currentRadius = currentRadius + heightStep * tan(lampAngle * DEG_TO_RADIANS);
	}

}

void Lamp::makeLamp(GLuint numlats, GLuint numlongs)
{
	GLuint i, j;
	
	GLuint numvertices = 1 + numlongs + (numlats * numlongs)*2;

	numlampvertices = numvertices;
	this->numlats = numlats;
	this->numlongs = numlongs;


	GLfloat* pVertices = new GLfloat[numvertices * 3];
	GLfloat* pColours = new GLfloat[numvertices * 4];
	GLfloat* pNormals = new GLfloat[numvertices * 3];
	getLampVertexPositions(pVertices, pNormals, pColours);



	glGenBuffers(1, &lampBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, lampBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &lampNormals);
	glBindBuffer(GL_ARRAY_BUFFER, lampNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &lampColours);
	glBindBuffer(GL_ARRAY_BUFFER, lampColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int numindices = (numlongs + 1 + 1) + (numlongs * 2 + 2)*(numlats - 1) * 2;
	GLuint* pindices = new GLuint[numindices];


	//fan on top
	pindices[0] = 0;
	i = 1;
	for (i; i <= numlongs;i++) {
		pindices[i] = i;
	}
	pindices[i] = 1;

	//strips on side!
	i++;
	for (int j = 0;j < 2;j++) {
		int currRow = 1;
		int indent = j*numlongs*numlats;
		for (int g = 0; g < numlats - 1; g++) {
			for (int k = 1;k <= numlongs;k++) {
				pindices[i] = indent + currRow*numlongs + k;
				pindices[i + 1] = indent + (currRow + 1)*numlongs + k;
				i = i + 2;
			}
			pindices[i] = indent + currRow*numlongs + 1;
			pindices[i + 1] = indent + (currRow + 1)*numlongs + 1;
			currRow++;
			i = i + 2;
		}
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




void Lamp::drawLamp(int drawmode, GLuint objectTypeID)
{



	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, lampBufferObject);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribute_v_coord);

	/* Bind the sphere normals */
	glBindBuffer(GL_ARRAY_BUFFER, lampNormals);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(attribute_v_normal);

	/* Bind the sphere colours */
	glBindBuffer(GL_ARRAY_BUFFER, lampColours);
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
		glDrawArrays(GL_POINTS, 0, numlampvertices);
	}
	else
	{
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		glDrawElements(GL_TRIANGLE_FAN, numlongs + 1 + 1, GL_UNSIGNED_INT, (GLvoid*)(0));//+1 for center vertex, +1 to close fan



		GLuint lat_offset_start = numlongs + 2;
		GLuint lat_offset_current = lat_offset_start * 4;
		GLuint lat_offset_jump = numlongs * 2 + 2;

		for (int i = 0; i < numlats - 1; i++) {
			glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
			lat_offset_current += (lat_offset_jump * 4);
		}

		glUniform1ui(objectTypeID, 4);

		for (int i = 0; i < numlats - 1; i++) {
			glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
			lat_offset_current += (lat_offset_jump * 4);
		}
	
	}
}

