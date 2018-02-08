#pragma once
#include <glm/glm.hpp>

struct Material
{
	//needs to be vec4 to make block uniforms happy
	glm::vec4 diffuseColor;
	float ambientPower;
	float specularReflectance;
	float specularPower;
};