#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;

// Ouput data
out vec3 color;

uniform mat4 V;

//Lights are in a uniform buffer
struct Light
{
	vec4 lightPositionWorldSpace;
	vec4 lightColor;
};
uniform light
{
	Light lights[10];
};

layout(std140) uniform material
{
	vec4 diffuseColor;
	float ambientPower;
	float specularReflectance;
	float specularPower;
};

void main(){

	
	//Add ambient lighting once
	vec3 ambientColor = ambientPower * diffuseColor.xyz;
	color = ambientColor;

	//loop over all lights and add them
	for(int i = 0; i < 10; i++){
		// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
		vec3 LightPosition_cameraspace = ( V * lights[i].lightPositionWorldSpace).xyz;
		vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

		// Distance to the light
		float dist = distance( lights[i].lightPositionWorldSpace.xyz, Position_worldspace );

		// Normal of the computed fragment, in camera space
		vec3 n = normalize( Normal_cameraspace );
		// Direction of the light (from the fragment to the light)
		vec3 l = normalize( LightDirection_cameraspace );
		float cosTheta = clamp( dot( n,l ), 0,1 );
		
		// Eye vector (towards the camera)
		vec3 E = normalize(EyeDirection_cameraspace);
		// Direction in which the triangle reflects the light
		vec3 R = reflect(-l,n);
		float cosAlpha = clamp( dot( E,R ), 0,1 );
		
		color += 
			// Diffuse : "color" of the object
			diffuseColor.xyz * lights[i].lightColor.xyz * lights[i].lightColor.w * cosTheta / (dist*dist) +
			// Specular : reflective highlight, like a mirror
			specularReflectance * lights[i].lightColor.xyz * lights[i].lightColor.w * pow(cosAlpha,specularPower) / (dist*dist);
	}
}