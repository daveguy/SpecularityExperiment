#pragma once
#include <glm/glm.hpp>

struct Material
{
	glm::vec3 diffuseColor;
	float ambientPower;
	float specularReflectance;
	float specularPower;
};