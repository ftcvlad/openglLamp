// Minimal fragment shader

#version 420 core

in vec4 fcolour;
out vec4 outputColor;
in vec3 N;//mv + normalized 
in vec3 light_pos3;
in vec3 lightDirection;
in vec4 P;//mv 

in float lampInnerSurface;
int  shininess = 8;


uniform uint emitmode;

vec3 global_ambient = vec3(0.05, 0.05, 0.05);
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);

vec3 diffuse_albedo = vec3(0, 0.2, 0);

varying vec4 shadowMapCoords0;
uniform sampler2D shadowMap;


float calcShadowAmount(){
	vec3 shadowMapCoords0Transformed = (shadowMapCoords0.xyz / shadowMapCoords0.w)*vec3(0.5) + vec3(0.5);
	
	if (texture(shadowMap, shadowMapCoords0Transformed.xy).r == 0){
		return 1;
	}
	else{
		return 0;
	}

	//if (texture2D(shadowMap, shadowMapCoords0Transformed.xy).r < shadowMapCoords0Transformed.z){
	//	return 0.0;
	//}
	//return 1.0;

}

void main()
{

	vec3 L = light_pos3 - P.xyz;//why if done in vertex shader does not work?????1		
	float distanceToLight = length(L);	
	L = normalize(L);		
	vec3 LD = normalize(lightDirection);		
	
	//ambient
	vec3 ambient = fcolour.xyz *0.2;

	//diffuse
	vec3 diffuse = max(dot(N, L), 0.0) * fcolour.xyz;
	//vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo.xyz;
	

	//specular
	vec3 V = normalize(-P.xyz);	
	vec3 R = reflect(-L, N);
	vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;

	//attenuation
	float attenuation_k = 0.0;
	float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2));
	//float attenuation = 1;
	//emission
	vec3 emissive = vec3(0);	

	if (emitmode == 1) emissive = vec3(1.0, 1.0, 0.8); 


	
	float cutoffAngleCos = cos(radians(35));
	float fullBrightnessAngleCos = cos(radians(20));
	

	//total
	float lightDirLCos = dot(-L, LD);
	
	if (lampInnerSurface == 1){//on inner surface behaves as point light
		outputColor = vec4(attenuation*(ambient + (diffuse + specular))+global_ambient, fcolour.w);
	}
	else if ( lightDirLCos>= cutoffAngleCos ){
		float coneAttenuation = 1;
		if (lightDirLCos <= fullBrightnessAngleCos){
			coneAttenuation = (lightDirLCos -  cutoffAngleCos) / (fullBrightnessAngleCos - cutoffAngleCos);
		}
		outputColor = vec4(attenuation*(ambient + coneAttenuation * (diffuse + specular))+global_ambient+emissive, fcolour.w);
	}
	else{
		outputColor = vec4(attenuation*(ambient)+global_ambient+emissive, fcolour.w);
	}

	//outputColor = outputColor * calcShadowAmount();

	float res = calcShadowAmount();
	if (res == 0){
		outputColor = vec4(0,1,0,1);
	}
	else{
		outputColor = vec4(0,0,1,1);
	}
	//outputColor = calcShadowAmount();
}