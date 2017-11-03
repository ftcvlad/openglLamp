
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

out vec3 light_pos3;



void main()
{
	

	vec4 position_h = vec4(position, 1.0);	// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)
	vec4 diffuse_albedo;					// This is the vertex colour, used to handle the colourmode change
	light_pos3 = lightpos.xyz;			

	// Switch the vertex colour based on the colourmode
	if (colourmode == 0)
		diffuse_albedo = colour;
	else
		diffuse_albedo = vec4(1.0, 0, 0, 1.0);




	N = normalize(normalmatrix * normal);// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
	
	if (objectType == 1){//lamp
		
		vec3 viewVector = normalize(vec3(0,0,-4));
		//-0.2 and not 0 because with 0 edge normals would be colored incorrectly. trial and error -0.2 :)
		//what if different lamp parameters???
		if (dot(viewVector, N) >= -0.2){
			diffuse_albedo = colour;
			N = N *-1;
		}
		else {
			diffuse_albedo = vec4(0,0,1,1);
			
		}
	}
	else{//floor
		diffuse_albedo = vec4(1,0,0,1);
	}

	


	// Define our vectors to calculate diffuse and specular lighting
	mat4 mv_matrix = view * model;		// Calculate the model-view transformation
	P = mv_matrix * position_h;	// Modify the vertex position (x, y, z, w) by the model-view transformation
	

	
	
	fcolour = diffuse_albedo;

	

	gl_Position = (projection * view * model) * position_h;
}

