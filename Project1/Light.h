#pragma once
#include <glm/glm.hpp>

struct Light
{
	glm::vec3 lightPosition_worldspace;
	glm::vec3 lightColor;
	float lightPower;
};