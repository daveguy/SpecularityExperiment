#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Ouput data
out vec3 color;

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

	
	
	vec3 ambientColor = ambientPower * diffuseColor.xyz;

	// Distance to the light
	float distance = length( lights[0].lightPositionWorldSpace.xyz - Position_worldspace );

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
	
	color = 
		// Ambient : simulates indirect lighting
		ambientColor +
		// Diffuse : "color" of the object
		diffuseColor.xyz * lights[0].lightColor.xyz * lights[0].lightColor.w * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		specularReflectance * lights[0].lightColor.xyz * lights[0].lightColor.w * pow(cosAlpha,specularPower) / (distance*distance);
}