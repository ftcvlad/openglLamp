/*
 poslight.cpp
 Demonstrates a positional light with attenuation
 Displays a cube and a sphere and a small sphere to show the light position
 Includes controls to move the light source and rotate the view
 Iain Martin October 2017
*/

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#pragma comment(lib, "glloadD.lib")
#else
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")
#endif
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
#include <stack>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <time.h>    
// Include headers for our objects
#include "sphere.h"
#include "lamp.h"
#include "line.h"
#include "floor.h"
#include "cube.h"
#include "bulb.h"
#include "wire.h"
/* Define buffer object indices */
GLuint elementbuffer;

GLuint program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */

GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader
					  I've included this to show you how to pass in an unsigned integer into
					  your vertex shader. */
GLuint objectType;
GLuint emitmode;

/* Position and view globals */
GLfloat angle_x, angle_inc_x, x, model_scale, z, y, vx, vy, vz, zAxRotationLamp;
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;

GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons
GLuint numlats, numlongs;	//Define the resolution of the sphere object

GLfloat bulbMoveX, bulbMoveY, bulbMoveZ;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID, lightdirID;
GLuint colourmodeID, emitmodeID, objectTypeID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/
GLuint numspherevertices;

/* Global instances of our objects */
Sphere aSphere;
Lamp aLamp;
Floor aFloor;
Cube aCube;
Bulb aBulb;
Wire aWire;


//swinging
bool lampSwinging = false;
int t2 = 0;
clock_t previousT = NULL;
clock_t currentT = NULL;
float maxAngle;
float PI = 3.1415926;
float g = 9.8;
float swingTimeCoefficient = 1600.0;
float swingStopCoefficient = 300.0;
float swingT;



using namespace std;
using namespace glm;

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	/* Set the object transformation controls to their initial values */
	x = 0;
	y = 0;
	z = 0;
	vx = 0; vx = 0, vz = 0.f;
	bulbMoveX = 0; bulbMoveY = 0; bulbMoveZ = 0;
	angle_x = angle_y = angle_z = 0;
	angle_inc_x = angle_inc_y = angle_inc_z = 0;
	model_scale = 1.f;
	aspect_ratio = 1.3333f;
	colourmode = 0; emitmode = 0;
	numlats = 200;		// Number of latitudes in our sphere
	numlongs = 200;		// Number of longitudes in our sphere
	zAxRotationLamp = 65.0;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Load and build the vertex and fragment shaders */
	try
	{
		program = glw->LoadShader("poslight.vert", "poslight.frag");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	colourmodeID = glGetUniformLocation(program, "colourmode");
	objectTypeID = glGetUniformLocation(program, "objectType");
	//1 lamp outer surface
	//2 floor
	//3 bulb glass
	//4 lamp inner surface
	//5 wire

	emitmodeID = glGetUniformLocation(program, "emitmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightposID = glGetUniformLocation(program, "lightpos");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");
	lightdirID = glGetUniformLocation(program, "lightdir");
	

	/* create our sphere and cube objects */
	aSphere.makeSphere(numlats, numlongs);
	aLamp.makeLamp(40, 80);
	aFloor.makeFloor();
	//aCube.makeCube();
	//aBulb.loadBulb();
	aWire.makeWire(40,30);
	swingT = 2 * PI * glm::sqrt(aWire.getWireLength() / g);
	// Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

}



  


void calcNewPendulumAngle() {
	
	if (previousT == NULL) previousT = clock();
	currentT = clock();
	t2 = t2 + (currentT - previousT);

	
	zAxRotationLamp = glm::degrees(glm::radians(maxAngle)*cos(2 * PI / swingT * (t2 / swingTimeCoefficient)));
	
	maxAngle -= (currentT - previousT) / swingStopCoefficient * (maxAngle>0? 1:-1);

	if (glm::abs(maxAngle) < 0.1) {
		maxAngle = 0;
		zAxRotationLamp = 0;
		lampSwinging = false;
	}
	previousT = currentT;
}



/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void display()
{


	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program);

	// Define our model transformation in a stack and 
	// push the identity matrix onto the stack
	stack<mat4> model;
	model.push(mat4(1.0f));

	// Define the normal matrix
	mat3 normalmatrix;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	mat4 projection = perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	mat4 view = lookAt(
		vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		vec3(0, 0, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// Apply rotations to the view position. This wil get appleid to the whole scene
	view = rotate(view, -vx, vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	view = rotate(view, -vy, vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	view = rotate(view, -vz, vec3(0, 0, 1));

	

	// Send our projection and view uniforms to the currently bound shader
	// I do that here because they are the same for all objects
	glUniform1ui(colourmodeID, colourmode);
	glUniform1ui(objectTypeID, objectType);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
	
	
	Line::drawLine(0, 3, 0, 0, 0, 0, "line1", modelID);

	
	// Define the global model transformations (rotate and scale). Note, we're not modifying thel ight source position
	model.top() = scale(model.top(), vec3(model_scale, model_scale, model_scale));//scale equally in all axis
	model.top() = rotate(model.top(), -angle_x, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model.top() = rotate(model.top(), -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model.top() = rotate(model.top(), -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis


	
	//model.push(model.top());
	//{
	//	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
	//	normalmatrix = transpose(inverse(mat3(view * model.top())));
	//	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);

	//	aCube.drawCube(drawmode);
	//}
	//model.pop();

	


	
	// WIRE
	model.push(model.top());
	{
	
		if (lampSwinging) {
			calcNewPendulumAngle();
		}
		

		model.top() = translate(model.top(), vec3(x, y + 1 + aWire.getWireLength(), z));
		model.top() = rotate(model.top(), zAxRotationLamp, glm::vec3(0, 0, 1));
		
		

		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		
		glUniform1ui(objectTypeID, 5);
		
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);

		aWire.drawWire(drawmode);
	}
	model.pop();


	//LAMP and BULB transformations
	model.push(model.top());
	{

		mat4 totalRotation = rotate(model.top(), zAxRotationLamp, glm::vec3(0, 0, 1));
		
		model.top() = translate(model.top(), vec3(x, y + 1 + aWire.getWireLength(), z));
		model.top() = rotate(model.top(), zAxRotationLamp, glm::vec3(0, 0, 1));
		model.top() = translate(model.top(), vec3(x, y- aWire.getWireLength(), z));
		


		vec4 lightDirection = totalRotation * vec4(0, -1, 0, 1.0);//only model rotations apply
		
		glUniform4fv(lightdirID, 1, value_ptr(lightDirection));

		
		
	}

	// LAMP
	model.push(model.top());
	{
		model.top() = scale(model.top(), vec3(0.5f, 0.5f, 0.5f));//scale equally in all axis

		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		glUniform1ui(objectTypeID, 1);
		
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);

		aLamp.drawLamp(drawmode, objectTypeID);
	}
	model.pop();


	//BULB
	model.push(model.top());
	{
		
		model.top() = translate(model.top(), vec3(0, -0.4, 0));
		model.top() = translate(model.top(), vec3(bulbMoveX, bulbMoveY, bulbMoveZ));
		model.top() = scale(model.top(), vec3(0.2f, 0.2f, 0.2f));
		

	

		// Define the light position and transform by the view matrix
		//vec4 lightpos = view * model.top() *  vec4(light_x, light_y, light_z, 1.0);
		vec4 lightpos = view * model.top() *  vec4(0, 1, 0, 1.0);//initially it was at 0, then depends on bulb's transformations
		glUniform4fv(lightposID, 1, value_ptr(lightpos));




		model.top() = rotate(model.top(), 180.0f, glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
																	   //model.top() = rotate(model.top(), -angle_y, glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
																	   //model.top() = rotate(model.top(), -angle_z, glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);

		glUniform1ui(objectTypeID, 3);
		//aBulb.drawBulb(drawmode, emitmodeID);




	}
	model.pop();
	
	Line::drawLine(0, 0, 0, 3, 0, 0, "line2", modelID);





	model.pop();//lamp and bulb transformations



	




	//FLOOR
	model.push(model.top());
	{

		model.top() = translate(model.top(), vec3(x, y-0.5, z));
		model.top() = scale(model.top(), vec3(20, 1, 20));//death #1

		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		glUniform1ui(objectTypeID, 2);//floor

		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);

		aFloor.drawFloor(drawmode);
	}
	model.pop();
	

	
	glDisableVertexAttribArray(0);
	glUseProgram(0);

	/* Modify our animation variables */
	angle_x += angle_inc_x;
	angle_y += angle_inc_y;
	angle_z += angle_inc_z;
}


/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/
	//if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == 'Q') angle_inc_x -= 0.05f;
	if (key == 'W') angle_inc_x += 0.05f;
	if (key == 'E') angle_inc_y -= 0.05f;
	if (key == 'R') angle_inc_y += 0.05f;
	if (key == 'T') angle_inc_z -= 0.05f;
	if (key == 'Y') angle_inc_z += 0.05f;
	if (key == 'A') model_scale -= 0.02f;
	if (key == 'S') model_scale += 0.02f;
	if (key == 'Z') x -= 0.05f;
	if (key == 'X') x += 0.05f;
	if (key == 'C') y -= 0.05f;
	if (key == 'V') y += 0.05f;
	if (key == 'B') z -= 0.05f;
	if (key == 'N') z += 0.05f;
	if (key == '1') bulbMoveX -= 0.05f;
	if (key == '2') bulbMoveX += 0.05f;
	if (key == '3') bulbMoveY -= 0.05f;
	if (key == '4') bulbMoveY += 0.05f;
	if (key == '5') bulbMoveZ -= 0.05f;
	if (key == '6') bulbMoveZ += 0.05f;
	if (key == '7') vx -= 1.f;
	if (key == '8') vx += 1.f;
	if (key == '9') vy -= 1.f;
	if (key == '0') vy += 1.f;
	if (key == 'O') vz -= 1.f;
	if (key == 'P') vz += 1.f;

	if (key == 'D' && lampSwinging == false) zAxRotationLamp -= 0.5f;
	if (key == 'F' && lampSwinging == false) zAxRotationLamp += 0.5f;
	if (key == 'L' && action != GLFW_PRESS) {
		lampSwinging = !lampSwinging;
		if (lampSwinging == true) {
			maxAngle = zAxRotationLamp;

			t2 = 0;
			previousT = NULL;
			currentT = NULL;
			
		}
	}

	if (key == 'M' && action != GLFW_PRESS)
	{
		colourmode = !colourmode;
	}

	/* Cycle between drawing vertices, mesh and filled polygons */
	if (key == ',' && action != GLFW_PRESS)
	{
		drawmode ++;
		if (drawmode > 2) drawmode = 0;
	}
}


/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Position light example");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	/* Output the OpenGL vendor and version */
	glw->displayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

