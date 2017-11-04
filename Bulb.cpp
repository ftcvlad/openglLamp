

#include "bulb.h"
#include "line.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_object_loader.h"

using namespace std;

Bulb::Bulb()
{
	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;

}

Bulb::~Bulb()
{
}




void Bulb::loadBulb()
{
	std::string inputfile = "models/bulb.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;


	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	numVertices = attrib.vertices.size();

	std::vector<tinyobj::real_t> pVertices = attrib.vertices;
	std::vector<tinyobj::real_t> pNormals = attrib.normals;
	std::vector<tinyobj::real_t> pColors = attrib.colors;



	numPIndexes = 0;
	for (size_t s = 0; s < shapes.size(); s++) {
		numPIndexes += shapes[s].mesh.num_face_vertices.size() * 3;//3 vertexes for each face
	}
	GLuint* pIndexes = new GLuint[numPIndexes];


	cout << shapes.size() << endl;
	cout << attrib.vertices.size() << endl;
	int ind = 0;

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {

		if (s == 2) {
			beginGlassIndex = ind;//to make glass of bulb emit color. shape 1 is glass colb
		}

		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
	
			int fv = shapes[s].mesh.num_face_vertices[f];//number of vertices per face

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				pIndexes[ind] = idx.vertex_index;
				ind++;

			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}

		if (s == 2) {
			endGlassIndex = ind - 1;
		}
	}







	glGenBuffers(1, &bulbBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bulbBufferObject);
	glBufferData(GL_ARRAY_BUFFER, pVertices.size() * sizeof(tinyobj::real_t), &pVertices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &bulbNormals);
	glBindBuffer(GL_ARRAY_BUFFER, bulbNormals);
	glBufferData(GL_ARRAY_BUFFER, pNormals.size() * sizeof(tinyobj::real_t), &pNormals.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &bulbColours);
	glBindBuffer(GL_ARRAY_BUFFER, bulbColours);
	glBufferData(GL_ARRAY_BUFFER, pColors.size() * sizeof(tinyobj::real_t), &pColors.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numPIndexes * sizeof(GLuint), pIndexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	////delete pNormals;
	delete pIndexes;
	//delete pColours;
	////delete pVertices;

}




void Bulb::drawBulb(int drawmode, GLuint emitmodeID)
{



	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, bulbBufferObject);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attribute_v_coord);

	/* Bind the sphere normals */
	glBindBuffer(GL_ARRAY_BUFFER, bulbNormals);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(attribute_v_normal);

	/* Bind the sphere colours */
	glBindBuffer(GL_ARRAY_BUFFER, bulbColours);
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




		glDrawElements(GL_TRIANGLE_STRIP, beginGlassIndex-1, GL_UNSIGNED_INT, (GLvoid*)(0));

		glUniform1ui(emitmodeID, 1);
		glDrawElements(GL_TRIANGLE_STRIP, endGlassIndex - beginGlassIndex, GL_UNSIGNED_INT, (GLvoid*)(beginGlassIndex*4));

		glUniform1ui(emitmodeID, 0);
		glDrawElements(GL_TRIANGLE_STRIP, numPIndexes - endGlassIndex, GL_UNSIGNED_INT, (GLvoid*)((endGlassIndex+1) * 4));
	}
}
