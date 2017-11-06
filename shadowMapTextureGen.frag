// Minimal fragment shader

#version 420 core

in vec4 fcolour;
out vec4 outputColor;

void main()
{

	float ndcDepth = 
		(2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /(gl_DepthRange.far - gl_DepthRange.near);
	float clipDepth = ndcDepth / gl_FragCoord.w;
	gl_FragColor = vec4((clipDepth * 0.5) + 0.5); 

}