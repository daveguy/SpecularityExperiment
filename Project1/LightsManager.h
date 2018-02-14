#pragma once
#include "GL/glew.h"
#include "glfw3.h"
#include <vector>
#include "Light.h"

class LightsManager
{
public:
	GLvoid* setUpLights(GLuint programID);

	void BindAndConnectLights(GLuint programID, int slot);

private:
	std::vector<Light> lights;
	GLuint lightBuffer;
	GLuint lightBlockID;
};