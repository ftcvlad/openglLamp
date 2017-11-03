

#include "floor.h"
#include "line.h"
#include <iostream>

using namespace std;

Floor::Floor()
{
	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;
}

Floor::~Floor()
{
}

void Floor::makeFloor()
{
	
	GLfloat pVertices[] = {
		-0.5, 0, 0.5,
		-0.5, 0, -0.5,
		0.5, 0, -0.5,

		- 0.5, 0, 0.5,
		0.5, 0, 0.5,
		0.5, 0, -0.5
	};

	GLfloat pNormals[] = {
		0,1,0,
		0,1,0,
		0,1,0,

		0,1,0,
		0,1,0,
		0,1,0
	};

	GLfloat pColors[] = {
		//0.38,0.38,0.38,1,
		//0.38,0.38,0.38,1,
		//0.38,0.38,0.38,1,
		0.5,0,0,1,
		0.5,0,0,1,
		0.5,0,0,1,
		0.5,0,0,1,
		0.5,0,0,1,
		0.5,0,0,1
	};

	nOfVertices = 6;

	glGenBuffers(1, &floorBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, floorBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);




	glGenBuffers(1, &floorNormals);
	glBindBuffer(GL_ARRAY_BUFFER, floorNormals);
	glBufferData(GL_ARRAY_BUFFER, nOfVertices * sizeof(glm::vec3), pNormals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &floorColours);
	glBindBuffer(GL_ARRAY_BUFFER, floorColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pColors), pColors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}




void Floor::drawFloor(int drawmode)
{



	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, floorBufferObject);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribute_v_coord);

	/* Bind the sphere normals */
	glBindBuffer(GL_ARRAY_BUFFER, floorNormals);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(attribute_v_normal);

	/* Bind the sphere colours */
	glBindBuffer(GL_ARRAY_BUFFER, floorColours);
	glVertexAttribPointer(attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribute_v_colours);

	

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, nOfVertices);
	}
	else
	{

		glBindBuffer(GL_ARRAY_BUFFER, floorBufferObject);
		glDrawArrays(GL_TRIANGLES, 0, nOfVertices);
	}
}