#pragma once

// Include GLFW
#include <glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Controls
{
public:
	Controls(glm::vec3 position, float horizontalAngle, float verticalAngle,
		float initialFoV, float speed, float scrollSpeed, float mouseSpeed, float initScroll)
		:position(position), horizontalAngle(horizontalAngle), verticalAngle(verticalAngle),
		initialFoV(initialFoV), speed(speed), scrollSpeed(scrollSpeed),
		mouseSpeed(mouseSpeed) {
		scroll = initScroll;
	}
	static void scrollFun(GLFWwindow *window, double xOffset, double yOffset);//callback function for field of view
	void computeMatricesFromInputs(GLFWwindow* window);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

private:
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	// Initial position : on +Z
	glm::vec3 position;
	// Initial horizontal angle : toward -Z
	float horizontalAngle;
	// Initial vertical angle : none
	float verticalAngle;
	// Initial Field of View
	float initialFoV;

	float speed; // 3 units / second
	float scrollSpeed;
	float mouseSpeed;
	static float scroll;
};