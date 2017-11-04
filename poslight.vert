
#version 420 core


layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;


out vec4 fcolour;

out vec3 N;
out vec4 diff_albedo;
//out vec3 L;
out vec4 P;
//Sout float distanceToLight;

uniform mat4 model, view, projection;
uniform mat3 normalmatrix;
uniform uint colourmode, emitmode, objectType;
uniform vec4 lightpos;
uniform vec4 lightdir;

out vec3 light_pos3;
out vec3 lightDirection;

out float lampInnerSurface;

void main()
{
	

	vec4 position_h = vec4(position, 1.0);	// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)
	vec4 diffuse_albedo;					// This is the vertex colour, used to handle the colourmode change
	light_pos3 = lightpos.xyz;			
	lightDirection = lightdir.xyz;

	// Switch the vertex colour based on the colourmode
	if (colourmode == 0)
		diffuse_albedo = colour;
	else
		diffuse_albedo = vec4(1.0, 0, 0, 1.0);




	N = normalize(normalmatrix * normal);// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
	
	
	diffuse_albedo = colour;
	lampInnerSurface = 0;
	if (objectType == 4){
		lampInnerSurface = 1.0;
	}	
	else if (objectType == 2){//floor
		diffuse_albedo = vec4(1,1,0,0.5);
	}
	else if (objectType == 3){
		diffuse_albedo = vec4(colour.x, colour.y, colour.z, 1);
	}

	


	// Define our vectors to calculate diffuse and specular lighting
	mat4 mv_matrix = view * model;		// Calculate the model-view transformation
	P = mv_matrix * position_h;	// Modify the vertex position (x, y, z, w) by the model-view transformation
	

	
	
	fcolour = diffuse_albedo;

	

	gl_Position = (projection * view * model) * position_h;
}


