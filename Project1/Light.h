#pragma once
#include <glm/glm.hpp>

///needs to be vec4 to make block uniforms happy
//lightPosition_worldspace, light color - xyz is color, w is power
struct Light
{
	glm::vec4 lightPosition_worldspace;
	glm::vec4 lightColor;//xyz is color, w is power
};