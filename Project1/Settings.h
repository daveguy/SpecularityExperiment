#pragma once

#include <glm/glm.hpp>

namespace settings
{
	//window settings (width, height)
	glm::vec2 winDimemsions(1024, 768);

	//Initial position and FoV for the camera controls
	// Initial position
	glm::vec3 initialPosition = glm::vec3(0.0f, 1.0f, 12.0f);
	// Initial horizontal angle : toward -Z
	float initialHorizontalAngle = 3.14f;
	// Initial vertical angle : none
	float initialVerticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	//move speed settings
	float moveSpeed = 3.0f; // 3 units / second
	float scrollSpeed = 8.0f;
	float mouseSpeed = 0.002f;

	glm::vec4 backgroundColor(0.0f, 0.0f, 0.4f, 0.0f);

	char* pathToObject = "surfaceLowAmp.obj";

	//initially the same as the initial camera position
	glm::vec3 specularityPositionStatic(initialPosition);
	glm::vec3 specularityPositionParallel(0.0f, 1.0f, 1000.0f);
}